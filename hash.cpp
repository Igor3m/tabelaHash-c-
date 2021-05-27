#include <iostream>
#include <cstdlib>

using namespace std;

typedef int Dado;

class Noh{
	friend class ListaDup;
	friend class Hash;
	private:
		Dado dado;
		Dado chave;
		Noh* proximo;
		Noh* anterior;
	public:
		Noh(Dado d=0, Dado c=0);
};
Noh::Noh(Dado d, Dado c){
	dado = d;
	chave = c;
	proximo = NULL;
	anterior = NULL;
}

class ListaDup{
	friend class Hash;
	private:
		Noh* primeiro;
		Noh* ultimo;
		int tamanho;
		void removeTodos();
	public:
		ListaDup();
		ListaDup(const ListaDup& umaLista);
		~ListaDup();
		ListaDup& operator =(const ListaDup& umaLista);
		inline void insere(Dado dado);
		void insereNoFim(Dado dado);
		void insereNoInicio(Dado dado);
		void insereNaPosicao(int posicao, Dado dado);
		int procura(Dado valor);
		void imprime();
		inline bool vazia();
		void removeNoInicio();
		void removeNoFim();
		void removeValor(Dado valor);
};
ListaDup::ListaDup(){
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}
ListaDup::ListaDup(const ListaDup& umaLista){
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
	Noh* aux = umaLista.primeiro;
	while(aux != NULL){
		insereNoFim(aux->dado);
		aux = aux->proximo;
	}
}
ListaDup::~ListaDup(){
	removeTodos();
}
void ListaDup::removeTodos(){
	Noh* aux = primeiro;
	Noh* temp;
	while (aux != NULL){
		temp = aux;
		aux = aux->proximo;
		delete temp;
	}
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}
ListaDup&ListaDup::operator =(const ListaDup& umaLista){
	removeTodos();
	Noh* aux = umaLista.primeiro;
	while(aux != NULL){
		insereNoFim(aux->dado);
		aux = aux->proximo;
	}
	return* this;
}
void ListaDup::insere(Dado dado){
	insereNoFim(dado);
}
void ListaDup::insereNoFim(Dado dado){
	Noh* novo = new Noh(dado);
	if (vazia()){
		primeiro = novo;
		ultimo = novo;
	}
	else{
		ultimo->proximo = novo;
		novo->anterior = ultimo;
		ultimo = novo;
	}
	tamanho++;
}
void ListaDup::insereNoInicio(Dado dado){
	Noh* novo = new Noh(dado);
	if (vazia()){
		primeiro = novo;
		ultimo = novo;
	}
	else {
		novo->proximo = primeiro;
		primeiro->anterior = novo;
		primeiro = novo;
	}
	tamanho++;
}
void ListaDup::insereNaPosicao(int posicao, Dado dado){
	Noh* novo = new Noh(dado);
	if ((posicao <= tamanho) and (posicao >= 0)){
		if (vazia()){
			primeiro = novo;
			ultimo = novo;
		}
		else if(posicao == 0){
			novo->proximo = primeiro;
			primeiro->anterior = novo;
			primeiro = novo;
		}
		else if(posicao == tamanho){
			ultimo->proximo = novo;
			novo->anterior = ultimo;
			ultimo = novo;
		}
		else{
			Noh* aux = primeiro;
			int posAux = 0;
			while(posAux < (posicao-1)){
				aux = aux->proximo;
				posAux++;
			}
			novo->proximo = aux->proximo;
			aux->proximo->anterior = novo;
			aux->proximo = novo;
			novo->anterior = aux;
		}
		tamanho++;
	}
	else {
		cerr << "Posicao inexistente" << endl;
		exit(EXIT_FAILURE);
	}
}
int ListaDup::procura(Dado valor){
	Noh* aux = primeiro;
	int posAux = 0;
	while((aux != NULL) and (aux->dado != valor)){
		posAux++;
		aux = aux->proximo;
	}
	if(aux == NULL){
		posAux = -1;
	}
	return posAux;
}
void ListaDup::imprime(){
	Noh* aux = primeiro;
	while(aux != NULL){
		cout << aux->dado << " ";
		aux = aux->proximo;
	}
	cout << endl;
	aux = ultimo;
	while(aux != NULL){
		cout << aux->dado << " ";
		aux = aux->anterior;
	}
	cout << endl;
}
inline bool ListaDup::vazia(){
	return (primeiro == NULL);
}
void ListaDup::removeNoInicio(){
	if (vazia()){
		cerr << "Remocao em lista vazia" << endl;
		exit(EXIT_FAILURE);
	}
	Noh* removido = primeiro;
	primeiro = primeiro->proximo;
	if (primeiro != NULL){
		primeiro->anterior = NULL;
	}
	delete removido;
	tamanho--;
	if (tamanho == 0){
		ultimo = NULL;
	}
}
void ListaDup::removeNoFim(){
	if (vazia()){
		cerr << "Remocao em lista vazia" << endl;
		exit(EXIT_FAILURE);
	}
	Noh* removido = ultimo;
	ultimo = ultimo->anterior;
	if (ultimo != NULL){
		ultimo->proximo = NULL;
	}
	delete removido;
	tamanho--;
	if (tamanho == 0){
		primeiro = NULL;
	}
}
void ListaDup::removeValor(Dado valor){
	Noh* aux = primeiro;
	while((aux != NULL) and (aux->dado != valor)){
		aux = aux->proximo;
	}
	if (aux == NULL){
		cerr << "Erro: Remocao de valor nao encontrado" << endl;
		exit(EXIT_FAILURE);
	}
	else{
		Noh* anterior = aux->anterior;
		Noh* proximo = aux->proximo;
		if (anterior != NULL){
			anterior->proximo = proximo;
		}
		else{
			primeiro = aux->proximo;
		}
		if (proximo != NULL){
			proximo->anterior = anterior;
		}
		else{
			ultimo = aux->anterior;
		}
		delete aux;
	}
	tamanho--;
	if (vazia()){
		primeiro = NULL;
		ultimo = NULL;
	}
}


class Hash{
	private:
		ListaDup* vetor;
		int capacidade;
		int calculaHash(Dado chave);
	public:
		Hash(int cap);
		~Hash();
		void insereElemento(Dado valor);
		void recuperaValor(Dado chave);
		void alteraElemento(Dado chave, Dado novoValor);
		void removeElemento(Dado chave);
		void redimensiona(int novaCapacidade);
		void procura(Dado valor);
		void imprime();

};
Hash::Hash(Dado cap){
	capacidade = cap;
	vetor = new ListaDup [cap];
}
Hash::~Hash(){
	delete vetor;
}
int Hash::calculaHash(Dado chave){
	return (chave%capacidade);
}
void Hash::insereElemento(Dado valor){
	Dado posicao = calculaHash(valor);
	ListaDup* tabela;
	tabela = vetor;
	if (tabela[posicao].procura(valor) == -1){
		tabela[posicao].insereNoFim(valor);
	}
	else{
		cerr << "Item ja esta na tabela" << endl;
		exit(EXIT_FAILURE);
	}
	
}
void Hash::recuperaValor(Dado chave){
	Dado posicao = calculaHash(chave);
	Dado elemento = vetor[posicao].procura(chave);
	if (elemento == -1){
		cerr << "Item nao encontrado" << endl;
		exit(EXIT_FAILURE);
	}
	else{
		recuperaValor(elemento);
	}
}
void Hash::alteraElemento(Dado chave, Dado novoValor){
	Dado posicao = calculaHash(chave);
	Dado elemento = vetor[posicao].procura(chave);
	if (elemento == -1){
		cerr << "Item nao encontrado" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		elemento = novoValor;
	}
}
void Hash::removeElemento(Dado chave){
	Dado posicao = calculaHash(chave);
	Dado elemento = vetor[posicao].procura(chave);
	if (elemento == -1){
		cerr << "Item nao encontrado" << endl;
		exit(EXIT_FAILURE);
	}
	else{
		vetor[posicao].removeValor(elemento);
	}
}
void Hash::redimensiona(int novaCapacidade){
}
void Hash::procura(Dado valor){
	Dado posicao = calculaHash(valor);
	Dado elemento = vetor[posicao].procura(valor);
	if (elemento == -1){
		cout << -1 << endl;
	}
	else{
		cout << 1 << endl;
	}
}
void Hash::imprime(){
	for (int i=0; i<capacidade; i++){
		cout << vetor[i];
	}
}
int main (){
	Dado capacidade = 10;
	Hash* tabela = new Hash(capacidade);
	Dado valor;
	for (int i=0; i<capacidade; i++){
		cin >> valor;
		tabela->insereElemento(valor);
	}
	tabela->imprime();
	return 0;
}

