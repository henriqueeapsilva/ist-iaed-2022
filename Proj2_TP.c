
/*
 * Segundo projeto de IAED
 * autor: ist103606
 * objetivo: construção de um sistema de voos entre aeroportos.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 6	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
    char id[MAX_CODIGO_AEROPORTO];
    char pais[MAX_NOME_PAIS];
    char cidade[MAX_NOME_CIDADE];
    int numVoos;
} Aeroporto;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int hora;
    int minuto;
} Hora;

typedef struct {
    char id[MAX_CODIGO_VOO];
    char partida[MAX_CODIGO_AEROPORTO];
    char chegada[MAX_CODIGO_AEROPORTO];
    Data data;
    Hora hora;
    Hora duracao;
    int capacidade;
    int horaPartida;
    int horaChegada;
} Voo;

typedef struct reserva{

    char *codigo_v, *codigo_r;
    Data data_reserva;
    int num_passageiros;
    struct reserva *prox; /* ponteiro para o proximo elemento da lista, inicializado a NULL */
} Reserva;


/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0		/* número de voos introduzidos */;
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

static Reserva* head; /* cabeça da lista de reservas */

/* Inicializa a head */

void inic(){
    head = NULL;
}

/* Funcoes Leitura */

Hora leHora() {
    Hora h;
    scanf("%d:%d", &h.hora, &h.minuto);
    return h;
}


Data leData() {
    Data d;
    scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
    return d;
}

int leProximaPalavra(char str[]) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();
    while (c != ' ' && c != '\t' && c != '\n') {
        str[i++] = c;
        c = getchar();
    }
    str[i] = '\0';
    return (c == '\n');
}

void lePalavraAteFimDeLinha(char str[]) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();
    while (c != '\n') {
        str[i++] = c;
        c = getchar();
    }
    str[i] = '\0';
}


void leAteFimDeLinha() {
    char c = getchar();
    while (c != '\n')
        c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
    if (d.dia < 10)
        printf("0");
    printf("%d-", d.dia);
    if (d.mes < 10)
        printf("0");
    printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
    if (h.hora < 10)
        printf("0");
    printf("%d:", h.hora);
    if (h.minuto < 10)
        printf("0");
    printf("%d", h.minuto);
}


int converteDataNum(Data d) {
    return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
           d.dia - 1;
}


int converteHoraNum(Hora h) {
    return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
    return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
    Hora h;
    h.minuto = num % MINUTOS_HORA;
    h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
    return h;
}


Data converteNumData(int num) {
    Data d;
    int i = 0;
    num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
    d.ano = (num / DIAS_ANO) + ANO_INICIO;
    num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
    while (i <= 11 && num >= _diasMesAc[i])
        i++;
    d.mes = i;
    d.dia = num - _diasMesAc[i - 1] + 1;
    return d;
}


int validaData(Data d) {
    int numData = converteDataNum(d);
    Data proximoAno = _hoje;
    proximoAno.ano++;
    return !(numData < converteDataNum(_hoje)
             || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
    return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
    int i, j, done;

    for (i = 0; i < size-1; i++){
        done=1;
        for (j = size-1; j > i; j--)
            if ((*cmpFunc)(indexes[j-1], indexes[j])) {
                int aux = indexes[j];
                indexes[j] = indexes[j-1];
                indexes[j-1] = aux;
                done=0;
            }
        if (done) break;
    }
}

/* Funcoes Aeroportos */

int aeroportoInvalido(char id[]) {
    int i, num_carac;
    for (i = 0; id[i] != '\0'; i++) {
        num_carac++;
        if(!(id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= 'a' && id[i] <= 'z'))
            return FALSE;
    }
    if(!(num_carac > 2 && num_carac < 6))
        return FALSE;
    return TRUE;
}

int encontraAeroporto(char id[]) {
    int i;
    for (i = 0; i < _numAeroportos; i++)
        if (!strcmp(id, _aeroportos[i].id))
            return i;
    return NAO_EXISTE;
}

void adicionaAeroporto() {
    Aeroporto a;

    leProximaPalavra(a.id);
    leProximaPalavra(a.pais);
    lePalavraAteFimDeLinha(a.cidade);

    if (aeroportoInvalido(a.id))
        printf("invalid airport ID\n");
    else if (_numAeroportos == MAX_NUM_AEROPORTOS)
        printf("too many airports\n");
    else if (encontraAeroporto(a.id) != NAO_EXISTE)
        printf("duplicate airport\n");
    else {
        strcpy(_aeroportos[_numAeroportos].id, a.id);
        strcpy(_aeroportos[_numAeroportos].pais, a.pais);
        strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
        _aeroportos[_numAeroportos].numVoos = 0;
        _numAeroportos++;
        printf("airport %s\n", a.id);
    }
}

void mostraAeroporto(int index) {
    printf("%s %s %s %d\n", _aeroportos[index].id,
           _aeroportos[index].cidade, _aeroportos[index].pais,
           _aeroportos[index].numVoos);
}

int cmpAeroportos(int a, int b) {
    return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}

void listaTodosAeroportos() {
    int i;
    int indexAeroportos[MAX_NUM_AEROPORTOS];

    for (i = 0; i < _numAeroportos; i++)
        indexAeroportos[i] = i;

    bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

    for (i = 0; i < _numAeroportos; i++)
        mostraAeroporto(indexAeroportos[i]);
}

void listaAeroportos() {
    char id[MAX_CODIGO_AEROPORTO];
    int indexAeroporto, ultima = 0;

    ultima = leProximaPalavra(id);
    if (strlen(id) == 0)
        listaTodosAeroportos();
    else {
        while (strlen(id) != 0) {
            indexAeroporto = encontraAeroporto(id);
            if (indexAeroporto == NAO_EXISTE)
                printf("%s: no such airport ID\n", id);
            else
                mostraAeroporto(indexAeroporto);
            if (!ultima)
                ultima = leProximaPalavra(id);
            else
                break;
        }
    }
}

/* Funcoes Voos */

void mostraVoo(int index) {
    printf("%s %s %s ", _voos[index].id, _voos[index].partida,
           _voos[index].chegada);
    mostraData(_voos[index].data);
    printf(" ");
    mostraHora(_voos[index].hora);
    printf("\n");
}

void mostraVooPartida(int index) {
    printf("%s %s ", _voos[index].id, _voos[index].chegada);
    mostraData(_voos[index].data);
    printf(" ");
    mostraHora(_voos[index].hora);
    printf("\n");
}

void mostraVooChegada(int index) {
    Hora h = converteNumHora(_voos[index].horaChegada);
    printf("%s %s ", _voos[index].id, _voos[index].partida);
    mostraData(converteNumData(_voos[index].horaChegada));
    printf(" ");
    mostraHora(h);
    printf("\n");
}

int encontraVoo(char id[], Data d) {
    int numData = converteDataNum(d);
    int i;

    for (i = 0; i < _numVoos; i++)
        if (!strcmp(id, _voos[i].id)
            && numData == converteDataNum(_voos[i].data))
            return i;
    return NAO_EXISTE;
}

int validaIDVoo(char id[]) {
    int i = 0, l = strlen(id);
    if (l < 3)
        return FALSE;
    for (i = 0; i < 2; i++)
        if (!(id[i] >= 'A' && id[i] <= 'Z'))
            return FALSE;

    while (id[i] != '\0') {
        if (!(id[i] >= '0' && id[i] <= '9'))
            return FALSE;
        i++;
    }
    return TRUE;
}

int validaVoo(Voo v) {
    if (validaIDVoo(v.id) == FALSE)
        printf("invalid flight code\n");
    else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
        printf("flight already exists\n");
    else if (encontraAeroporto(v.partida) == NAO_EXISTE)
        printf("%s: no such airport ID\n", v.partida);
    else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
        printf("%s: no such airport ID\n", v.chegada);
    else if (_numVoos == MAX_NUM_VOOS)
        printf("too many flihts\n");
    else if (validaData(v.data) == FALSE)
        printf("invalid date\n");
    else if (validaHora(v.duracao) == FALSE)
        printf("invalid duration\n");
    else if (v.capacidade < 10)
        printf("invalid capacity\n");
    else
        return TRUE;
    return FALSE;
}

void criaVoo(Voo v) {
    strcpy(_voos[_numVoos].id, v.id);
    strcpy(_voos[_numVoos].partida, v.partida);
    strcpy(_voos[_numVoos].chegada, v.chegada);
    _voos[_numVoos].data = v.data;
    _voos[_numVoos].hora = v.hora;
    _voos[_numVoos].duracao = v.duracao;
    _voos[_numVoos].capacidade = v.capacidade;
    _voos[_numVoos].horaPartida =
            converteDataHoraNum(_voos[_numVoos].data,
                                _voos[_numVoos].hora);
    _voos[_numVoos].horaChegada =
            _voos[_numVoos].horaPartida +
            converteHoraNum(_voos[_numVoos].duracao);
    _numVoos++;
}

void adicionaListaVoos() {
    Voo v;
    int i;

    if (leProximaPalavra(v.id)) {
        for (i = 0; i < _numVoos; i++)
            mostraVoo(i);
        return;
    } else {
        leProximaPalavra(v.partida);
        leProximaPalavra(v.chegada);
        v.data = leData();
        v.hora = leHora();
        v.duracao = leHora();
        scanf("%d", &v.capacidade);
        leAteFimDeLinha();
    }

    if (validaVoo(v))
        criaVoo(v);
}

int cmpVoosPartida(int a, int b) {
    return (_voos[a].horaPartida > _voos[b].horaPartida);
}

int cmpVoosChegada(int a, int b) {
    return (_voos[a].horaChegada > _voos[b].horaChegada);
}

void listaVoosPartida() {
    int indexVoos[MAX_NUM_VOOS], i, n = 0;
    char partida[MAX_CODIGO_AEROPORTO];

    lePalavraAteFimDeLinha(partida);

    if (encontraAeroporto(partida) == NAO_EXISTE)
        printf("%s: no such airport ID\n", partida);

    for (i = 0; i < _numVoos; i++) {
        if (!strcmp(_voos[i].partida, partida))
            indexVoos[n++] = i;
    }

    bubbleSort(indexVoos, n, cmpVoosPartida);

    for (i = 0; i < n; i++) {
        if (converteDataNum(_voos[indexVoos[i]].data) == converteDataNum(_hoje))
            mostraVooPartida(indexVoos[i]);
    }
}

void listaVoosChegada() {
    int indexVoos[MAX_NUM_VOOS], i, n = 0;
    char chegada[MAX_CODIGO_AEROPORTO];

    lePalavraAteFimDeLinha(chegada);

    if (encontraAeroporto(chegada) == NAO_EXISTE)
        printf("%s: no such airport ID\n", chegada);

    for (i = 0; i < _numVoos; i++) {
        if (!strcmp(_voos[i].chegada, chegada))
            indexVoos[n++] = i;
    }

    bubbleSort(indexVoos, n, cmpVoosChegada);

    for (i = 0; i < n; i++)
        mostraVooChegada(indexVoos[i]);
}

/* funcao que altera a data do programa */
void alteraData() {
    Data d;

    d = leData();
    leAteFimDeLinha();
    if (validaData(d) == FALSE)
        printf("invalid date\n");
    else {
        _hoje = d;
        mostraData(_hoje);
        printf("\n");
    }
}

/* funcao que após a adição de uma reserva retira a capacidade disponivel do voo em questão */
void retira_capacidade_voo(char* id, Data d, int num){
    int i;
    if((i = encontraVoo(id,d)) != NAO_EXISTE)
        _voos[i].capacidade -= num;
}

/* funcao que após a eliminação uma reserva adiciona a capacidade disponivel do voo em questão */
void adiciona_passgeiros(Reserva* aux){
    int i;
    i = encontraVoo(aux->codigo_v, aux->data_reserva);
    _voos[i].capacidade += aux->num_passageiros;
}

/* Funçao verifica se lista e vazia */

int lista_vazia(){
    return (head == NULL);
}

/* Funcoes memoria */

/* liberta toda a memoria alocada a uma unica reserva */
void liberta_reserva(Reserva* temp){
    free(temp->codigo_r);
    free(temp->codigo_v);
    free(temp);
}

/* liberta toda a memoria alocada na lista de reservas */
void todos_free(){
    Reserva *aux;
    while (head != NULL){
        aux = head;
        head = head->prox;
        liberta_reserva(aux);
    }
}

/* acabou a memoria */
void acabou_memoria(){
    printf("no memory\n");
    todos_free();    /* caso nao haja o programa da free a toda a memoria alocada e da exit */
    exit(1);
}
/* Funcoes Reservas */

/* verifica se exista um voo para uma dada reserva a adicionar numa certa data,
   retornando caso afirmativo a capacidade do voo. */
int existe_data_voo_reserva(Data t,char* codigo_voo){
    int i;
    if((i = encontraVoo(codigo_voo,t)) != NAO_EXISTE)
        return _voos[i].capacidade;
    return NAO_EXISTE;
}

/* verifica se ja existe uma reserva com o mesmo id de reserva */
int existe_reserva(char* cod_r){
    Reserva* aux;
    if(head == NULL)
        return 0;
    for(aux = head; aux != NULL; aux = aux->prox){
        if(strcmp(aux->codigo_r,cod_r) == 0)
            return 1;
    }
    return 0;
}

/* verifica se o codigo de reserva esta de acordo com o pretendido */
int codigo_errado(char* codigo_r){
    int i, tamanho = strlen(codigo_r);
    if(tamanho < 10)
        return 1;
    for(i = 0;i<tamanho; i++)
        if ((codigo_r[i] < 'A' || codigo_r[i] > 'Z') && (codigo_r[i] < '0' || codigo_r[i] > '9'))
            return 1;
    return 0;
}

/* funcao faz todas as verificaçoes necessarias para que a reserva seja posteriormente adicionada */
int verifica_reserva(char* codigo_r, char* codigo_voo , Data t, int num_passageiros) {
    int passageiros;

    if(codigo_errado(codigo_r) == 1)
        printf("invalid reservation code\n");
    else if((passageiros = existe_data_voo_reserva(t,codigo_voo)) == NAO_EXISTE)
        printf("%s: flight does not exist\n", codigo_voo);
    else if(existe_reserva(codigo_r) == 1)
        printf("%s: flight reservation already used\n", codigo_r);
    else if(num_passageiros > passageiros)
        printf("too many reservations\n");
    else if(!validaData(t))
        printf("invalid date\n");
    else if(num_passageiros <= 0)
        printf("invalid passenger number\n");
    else
        return 0;
    return 1;
}

/* funcao qur cria uma reserva alocando memoria para a reserva e para os parametros
  codigo de voo e codigo de reserva da mesma */
Reserva* Cria_nova_reserva(char* codigo_voo,char* codigo_r ,Data data_reserva, int num_passageiros) {
    Reserva* nova_reserva = (Reserva*) malloc(sizeof(Reserva));
    if(nova_reserva == NULL)          /* verifica se ainda existe memoria suficiente para alocar uma nova reserva */
        acabou_memoria();
    nova_reserva->codigo_r = (char*) malloc(sizeof(char) * (strlen(codigo_r) + 1));
    strcpy(nova_reserva->codigo_r, codigo_r);
    nova_reserva->codigo_v = (char*) malloc(sizeof(char) * (strlen(codigo_voo) + 1));
    strcpy(nova_reserva->codigo_v, codigo_voo);
    nova_reserva->data_reserva.dia = data_reserva.dia;
    nova_reserva->data_reserva.mes = data_reserva.mes;
    nova_reserva->data_reserva.ano = data_reserva.ano;
    nova_reserva->num_passageiros = num_passageiros;
    nova_reserva->prox = NULL;
    return nova_reserva;
}

/* funcao que adiciona a nova reserva a lista de reservas ordenadas lexicamente */
void adiciona_lista(char* codigo_voo,char* codigo_r ,Data data_reserva, int num_passageiros){
    Reserva *Reserva_adicionar, *aux;
    if(verifica_reserva(codigo_r, codigo_voo,data_reserva ,num_passageiros) == 0) {
        if (head == NULL)   /* caso não haja reservas */
            head = Cria_nova_reserva(codigo_voo, codigo_r, data_reserva, num_passageiros);
        else if (strcmp(codigo_r, head->codigo_r) < 0) {
            Reserva_adicionar = Cria_nova_reserva(codigo_voo, codigo_r, data_reserva, num_passageiros);
            Reserva_adicionar->prox = head;   /* caso o codigo de reserva a adicionar seja 'menor lexicamente', adiciona na primeira psoicao */
            head = Reserva_adicionar;
        } else {
            for (Reserva_adicionar = head, aux = Reserva_adicionar->prox;
                 Reserva_adicionar->prox != NULL; Reserva_adicionar = Reserva_adicionar->prox, aux = aux->prox)
                if (strcmp(codigo_r, aux->codigo_r) < 0) {   /* adiciona no meio da lista de reservas ordenadamente */
                    Reserva_adicionar->prox = Cria_nova_reserva(codigo_voo, codigo_r, data_reserva, num_passageiros);
                    Reserva_adicionar->prox->prox = aux;
                    break;
                }
            if (aux == NULL) /* se chegou ao fim da lista, adiciona o elemneto na ultima posicao da lista */
                Reserva_adicionar->prox = Cria_nova_reserva(codigo_voo, codigo_r, data_reserva, num_passageiros);
        }
        retira_capacidade_voo(codigo_voo, data_reserva, num_passageiros);
    }
}

/* funcao que le o numero de passageiros da reserva e chama a funcao adiciona_lista */
void func_r_adiciona(char *codigo_voo ,char *cod_r,Data t){
    int num_passageiros;
    scanf("%d", &num_passageiros);
    adiciona_lista(codigo_voo, cod_r, t, num_passageiros);
}

/* funcap que vai escrevas todas as reservas lexicamente ordenadas que tenham o codigo de voo e data dadas no input*/
void func_r_escreve(char *codigo_voo, Data data_voo){
    Reserva* aux;
    int data;
    data = converteDataNum(data_voo);
    for(aux = head; aux != NULL ; aux = aux->prox){
        if(strcmp(aux->codigo_v, codigo_voo) == 0 && converteDataNum(aux->data_reserva) == data) {
            printf("%s %d\n", aux->codigo_r, aux->num_passageiros);

        }
    }
}

/* funcao que dinamiza toda a funcao 'r' e permite distinguir que funcionalidade sera executada (adicionar ou escrever) */
void func_r(){
    char cod_voo[MAX_CODIGO_VOO], cod_r[65535];
    Data t;
    leProximaPalavra(cod_voo);
    t = leData();
    if(leProximaPalavra(cod_r)) {
        if(validaData(t)) {
            if(encontraVoo(cod_voo, t) != NAO_EXISTE)
                func_r_escreve(cod_voo, t);
            else
                printf("%s: flight does not exist\n", cod_voo);
        }
        else
            printf("invalid date\n");
    }
    else
        func_r_adiciona(cod_voo, cod_r, t);
}

/* Funcoes comando e */

/* funcao que dada uma*/
void apaga_reserva(char* buffer) {
    Reserva *aux,*temp = NULL;
    int done = 0;   /* verifica se foi eliminada alguma reserva */
    if (!lista_vazia()) {
        if (head != NULL && !strcmp(head->codigo_r, buffer)) {
            aux = head;
            adiciona_passgeiros(aux);       /* caso a reserva a eliminar seja a primeira é eliminada */
            head = head->prox;
            liberta_reserva(aux);
            done++;
        }
        aux = head;
        if(aux != NULL && done == 0)
            temp = aux->prox;
        while(temp != NULL && done == 0) {   /* para o resto da lista vai comparando se são diferentes o codigo de reserva com o buffer e caso não o sejam é eiminada a reserva */
            if (strcmp(temp->codigo_r, buffer)){
                aux = temp, temp = aux->prox;
            }
            else{
                aux->prox = temp->prox;
                adiciona_passgeiros(temp);
                liberta_reserva(temp);
                aux = temp->prox;
                done++;
                if(aux != NULL)
                    temp = aux->prox;
                else
                    temp = aux;
            }
        }
    }
    if(done == 0)
        printf("not found\n");

}

/* funcao que elimina todas as reservas com um dado codigo de voo */
void remove_reservasVoo(char* buffer) {
    Reserva *aux, *temp = NULL;
    if (!lista_vazia()) {
        while (head != NULL && !strcmp(head->codigo_v, buffer)) {
            aux = head;
            adiciona_passgeiros(aux);
            head = head->prox;
            liberta_reserva(aux);
        }
        aux = head;
        if(aux!=NULL)
            temp = aux->prox;
        while (temp != NULL) {
            if (strcmp(temp->codigo_v, buffer)) {
                aux = temp;
                temp = aux->prox;
            } else {
                aux->prox = temp->prox;
                adiciona_passgeiros(temp);
                liberta_reserva(temp);
                aux = temp->prox;
                if (aux != NULL)
                    temp = aux->prox;
                else
                    temp = aux;
            }
        }
    }
}
/* funcao que retira o voo e as reservas correspondentes ao voo */
void apaga_voo_reservas(char* buffer){
    int i,j, done = 0;
    if (_numVoos == 1) {
        if (strcmp(buffer, _voos[0].id) == 0 && (converteDataNum(_voos[0].data) < converteDataNum(_hoje))) {
            _numVoos--;
            done = 1;
        }
    }
    else
        for(i=0;i<_numVoos;i++) {
            if (strcmp(buffer, _voos[i].id) == 0 && (converteDataNum(_voos[0].data) < converteDataNum(_hoje))) {
                for (j = i; j+1 <= _numVoos; j++) {
                    _voos[j] = _voos[j+1];
                    done = 1;
                }
                _numVoos--;
                i--;
            }
        }

    if(done != 0)
        remove_reservasVoo(buffer);
    else
        printf("not found\n");

}

/* funcao que dinamiza toda a funcao 'e' e permite distinguir que funcionalidade sera executada (eliminar uma reserva ou um voo e as suas reservas) */
void func_elimina(){
    char buffer[65535];
    leProximaPalavra(buffer);
    if(!codigo_errado(buffer))
        apaga_reserva(buffer);
    else if(validaIDVoo(buffer) == TRUE)
        apaga_voo_reservas(buffer);
    else
        printf("not found\n");
}

int main() {
    int c;

    while ((c = getchar()) != EOF) {

        switch (c) {
            case 'q': todos_free();
                return 0;
            case 'a': adicionaAeroporto();
                break;
            case 'l': listaAeroportos();
                break;
            case 'v': adicionaListaVoos();
                break;
            case 'p': listaVoosPartida();
                break;
            case 'c': listaVoosChegada();
                break;
            case 't': alteraData();
                break;
            case 'r': func_r();
                break;
            case 'e': func_elimina();
                break;
        }
    }
    return 0;
}