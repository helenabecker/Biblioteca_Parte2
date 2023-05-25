#include <iostream>  // Entrada e saida de dados padrao
#include <string> // Estamos usando getline
#include <time.h> // Srand
#include <ctime> // Funcao para mostrar a data atual do programa
#include <Windows.h> // Funcoes set_cursor && set_color
using namespace std;

// Trabalho feito por Flávia Schnaider e Helena Becker Piazera

// LINK DA APRESENTAÇÃO: https://1drv.ms/a/s!Ag7T0yMC71dPgpQ6V2sFk_tO1MNhaw?e=wRdgKu

const int MAX_RETIRADOS = 100;

#pragma region Structs

struct Data {
	int dia, mes, ano;
};

struct Autor {
	string nome, sobrenome;
};

struct Editora {
	string nome, local;
};

struct Retirado {
	int id = 0;
	Data retirada, devolucao;
	bool atraso = false;
};

struct Usuario {
	string nome;
	int id = 0;
	Retirado retirados[100];
};

struct Livro {
	string titulo, assunto;
	Autor autor;
	Editora editora;
	int publicacao, id = 0;
	bool disponivel = true;
	Data retirada;
};

struct Revista {
	string titulo, assunto;
	Editora editora;
	Data publicacao;
	int id = 0;
	bool disponivel = true;
};

#pragma endregion Structs

#pragma region Funcoes

//Usado para posicionar o cursor em uma determinada posição da tela
void set_cursor(int x = 0, int y = 0) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//system("color 1"); também funciona vai de 1 a 15
void set_color(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COLORREF cor;
	SetConsoleTextAttribute(handle, color);
}

int opcao_invalida(string frase, int min, int max) {
	int op;
	do {
		cout << frase << " (" << min << " - " << max << "): ";
		cin >> op;
		if (op < min || op > max) {
			set_color(4);
			cout << "\nOpcao invalida. Tente novamente." << endl;
			Sleep(500);
			set_color(7);
		}
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			op = min - 1;
		}
	} while (op < min || op > max);
	return op;
}

Data ler_data() {
	Data d;
	do {
		cout << "\tDia: ";
		cin >> d.dia;
		cout << "\tMes: ";
		cin >> d.mes;
		cout << "\tAno";
		cin >> d.ano;
	} while (d.dia > 31 || d.mes < 1 || d.mes > 12 || d.ano < 1900 || d.ano > 2023);

	return d;
}

void mostrar_data(Data d) {
	if (d.dia < 10) {
		cout << "0";
	}
	cout << d.dia << "/";
	if (d.mes < 10) {
		cout << "0";
	}
	cout << d.mes << "/" << d.ano;
}

int dias_no_mes(int mes, int ano) {
	if (mes == 2) {
		if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
			return 29; // ano bissexto
		}
		else {
			return 28; // ano não bissexto
		}
	}
	if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
		return 30; // meses com 30 dias
	}
	if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 10 || mes == 12) {
		return 31; // meses com 31 dias
	}
}

Data data_devolucao(Data data_retirada) {
	Data data_devolucao;
	int dias_no_mes_atual;

	data_devolucao.dia = data_retirada.dia + 7;
	data_devolucao.mes = data_retirada.mes;
	data_devolucao.ano = data_retirada.ano;

	dias_no_mes_atual = dias_no_mes(data_devolucao.mes, data_devolucao.ano);

	if (data_devolucao.dia > dias_no_mes_atual) {
		data_devolucao.dia -= dias_no_mes_atual;
		data_devolucao.mes++;
		if (data_devolucao.mes > 12) {
			data_devolucao.mes = 1;
			data_devolucao.ano++;
		}
	}
	return data_devolucao;
}

//calcula os dias passados entre duas datas
int dias_passados(Data retirada, Data data) {
	int dias = 0;
	while (data.dia != retirada.dia || data.mes != retirada.mes) {
		if (data.dia > 1) {
			data.dia--;
			dias++;
		}
		else if (data.dia == 1) {
			dias++;
			data.mes--;
			data.dia = dias_no_mes(data.mes, data.ano);
		}
		else if (data.mes > 1) {
			data.mes--;
			data.dia = dias_no_mes(data.mes, data.ano);
		}
		else {
			data.ano--;
			data.mes = 12;
			data.dia = dias_no_mes(data.mes, data.ano);
		}
	}
	return dias;
}

void mostrar_autor(Autor a) {
	cout << a.nome << " " << a.sobrenome << endl;
}

void mostrar_editora(Editora ed) {
	cout << ed.nome << ", " << ed.local << endl;
}

void mostrar_livro(Livro a) {
	cout << "\nDados do livro " << a.id << ":\n" << endl
		<< "\tTitulo: " << a.titulo << endl;

	cout << "\tAutor: ";
	mostrar_autor(a.autor);
	cout << "\tEditora: ";
	mostrar_editora(a.editora);

	cout << "\tAno de Publicacao: ";
	if (a.publicacao < 10) {
		cout << "0";
	}
	cout << a.publicacao << endl;
	cout << "\tAssunto: " << a.assunto << endl;

	if (a.disponivel == true) {
		cout << "\tStatus: Disponivel";
	}
	else {
		cout << "\tStatus: Indisponivel";
	}
	cout << endl;
}

void mostrar_revista(Revista r) {
	cout << "\nDados da Revista " << r.id << ":\n" << endl
		<< "\tTitulo: " << r.titulo << endl

		<< "\tEditora: ";
	mostrar_editora(r.editora);

	cout << "\tData da Publicacao: ";
	if (r.publicacao.mes < 10) {
		cout << "0";
	}
	cout << r.publicacao.mes << "/" << r.publicacao.ano << endl;
	cout << "\tAssunto: " << r.assunto << endl;

	if (r.disponivel == true)
		cout << "\tStatus: Disponivel\n";
	else {
		cout << "\tStatus: Indisponivel\n";
	}
}

void mostrar_usuario(Usuario u) {
	cout << "\nDados do Usuario: " << endl
		<< "\tID do usuario: " << u.id << endl
		<< "\tNome Completo: " << u.nome << endl;
}

Autor ler_autor() {
	Autor autor;
	cout << "Nome do Autor: ";
	getline(cin, autor.nome);

	cout << "Sobrenome do Autor: ";
	getline(cin, autor.sobrenome);

	return autor;
}

Editora ler_editora() {
	Editora editora;
	cout << "Nome da Editora: ";
	getline(cin, editora.nome);

	cout << "Local da Editora: ";
	getline(cin, editora.local);

	return editora;
}

bool verificador_autor(int cont_autor, Autor lista_autores[], Autor autor) { //verifica se ja existe cadastro deste autor
	for (int i = 0; i < cont_autor + 1; i++) {
		if (autor.nome == lista_autores[i].nome && autor.sobrenome == lista_autores[i].sobrenome) {
			set_color(4);
			cout << "\n\tAutor ja cadastrado no sistema" << endl;
			return true;
			break;
		}
	}
	return false;
}

bool verificador_editora(int cont_editora, Editora lista_editoras[], Editora ed) { //verifica se ja existe cadastro desta editora
	for (int i = 0; i < cont_editora + 1; i++) {
		if (ed.nome == lista_editoras[i].nome && ed.local == lista_editoras[i].local) {
			set_color(4);
			cout << "\n\tEditora ja cadastrada no sistema" << endl;
			return true;
			break;
		}
	}
	return false;
}

Usuario cadastrar_usuario() {
	Usuario u;
	cin.ignore();
	cout << "\nNome Completo: ";
	getline(cin, u.nome);

	return u;
}

Livro cadastrar_livro(Autor lista_autores[], int cont_autor, Editora lista_editoras[], int cont_editora, int id_livro, Data data_atual) {
	Livro l;
	int escolha;
	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, l.titulo);

	l.publicacao = opcao_invalida("Ano de publicacao", 0, data_atual.ano);

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, l.assunto);

	//mostra lista de autores e so permite escolha de um autor ja cadastrado
	set_color(6);
	cout << "\nLista de autores:" << endl;
	set_color(7);
	for (int i = 0; i < cont_autor + 1; i++) {
		cout << "\t[" << i + 1 << "] ";
		mostrar_autor(lista_autores[i]);
	}
	escolha = opcao_invalida("\nEscolha o autor", 1, cont_autor + 1);
	l.autor = lista_autores[escolha - 1];

	//mostra lista de editoras e so permite escolha de uma editora ja cadastrada
	set_color(6);
	cout << "\nLista atual de editoras cadastradas:" << endl;
	set_color(7);
	for (int i = 0; i < cont_editora + 1; i++) {
		cout << "\t[" << i + 1 << "] ";
		mostrar_editora(lista_editoras[i]);
	}
	escolha = opcao_invalida("\nEscolha a editora", 1, cont_editora + 1);
	l.editora = lista_editoras[escolha - 1];

	l.id = id_livro;
	return l;
}

Revista cadastrar_revista(Editora lista_editoras[], int cont_editora, Data data_atual, int id_revista) {
	Revista r;
	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, r.titulo);

	//mostra lista de editoras e so permite escolha de uma editora ja cadastrada
	set_color(6);
	cout << "\nLista de editoras: \n" << endl;
	set_color(7);
	for (int i = 0; i < cont_editora + 1; i++) {
		cout << "\t[" << i + 1 << "] ";
		mostrar_editora(lista_editoras[i]);
	}
	int escolha = opcao_invalida("\nEscolha a editora", 1, cont_editora + 1);
	r.editora = lista_editoras[escolha - 1];

	r.publicacao.ano = opcao_invalida("\n\nAno de publicacao", 0, data_atual.ano); //conferencia de ano valido
	if (r.publicacao.ano == data_atual.ano) {
		r.publicacao.mes = opcao_invalida("\nMes de publicacao", 1, data_atual.mes); //conferencia de mes valido caso seja do ano atual
	}
	else {
		r.publicacao.mes = opcao_invalida("\nMes de publicacao", 1, 12);
	}

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, r.assunto);

	r.id = id_revista;
	return r;
}

//quantidade de itens retirados por um usuario
int qtd_retiradas(Usuario usuario) {
	int qtd = 0;
	for (int j = 0; j < MAX_RETIRADOS; j++) {
		if (usuario.retirados[j].id != 0) { //verifica a quantidade de itens já retirados pelo usuario
			qtd++;
		}
	}
	return qtd;
}

//verifica se ha atraso 
bool atraso(Usuario usuario) {
	for (int i = 0; i < MAX_RETIRADOS; i++) {
		if (usuario.retirados[i].atraso) { //nao permite retirar mais livros enquanto tiver um com atraso de devolucao
			Data data_atual = ler_data();
			if (data_atual.ano > usuario.retirados[i].devolucao.ano ||
				(data_atual.ano == usuario.retirados[i].devolucao.ano &&
					(data_atual.mes > usuario.retirados[i].devolucao.mes ||
						(data_atual.mes == usuario.retirados[i].devolucao.mes &&
							data_atual.dia > usuario.retirados[i].devolucao.dia)))) {
				set_color(4);
				cout << "\nVoce tem um item com atraso." << endl
					<< "Nao podera realizar mais retiradas enquanto nao regularizar as devolucoes" << endl;
				set_color(7);
				return true;
			}
		}
	}
	return false;
}

int gerar_ID(int num) {
	int id = 0;
	srand(time(NULL));
	if (num < 1100) {
		id = num + rand() % 900 + 1; //id pra livros
		return id;
	}
	if (num > 1100 && num < 2100) {
		id = num + rand() % 900 + 1; //id pra revistas
		return id;
	}
	if (num > 2100 && num < 3100) {
		id = num + rand() % 900 + 1; //id pra usuarios
		return id;
	}
}

//gera cadastros iniciais de titulos
void gerar_titulos(string lista_titulos[], string titulos_revistas[]) {
	const int tam = 5, tam2 = 10;
	string titulos[] = { "Tudo ou Nada", "Amanha eh outro dia", "Felizes para Sempre", "A Ultima Batalha", "Guerra dos Reis", "A Voz da Natureza", "Canto da Tempestade", "Maldicao Azul", "Harmonia em Vermelho", "Deuses em Caos" };
	bool flag = false;
	int aux[tam] = { 0 }, a = 0;

	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % tam2;
		if (i == 0) {
			lista_titulos[i] = titulos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o titulo de livro ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_titulos[i] = titulos[a];
				aux[i] = a;
			}
		}
	}
	const int tam_r = 4, tam2_r = 10;
	int aux2[tam_r] = { 0 }, b = 0;
	string titulos_r[] = { "Vogue", "ELLE", "V Magazine", "Capricho", "Caras", "Veja SP", "A Sentinela", "Despertai", "Crusoe Revista Digital Semanal", "Manequim" };

	srand(time(NULL));
	for (int i = 0; i < tam_r; i++) {
		b = rand() % tam2_r;
		if (i == 0) {
			titulos_revistas[i] = titulos_r[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o titulo de revista ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				titulos_revistas[i] = titulos_r[b];
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de autores
void gerar_autores(Autor lista_autores[]) {
	const int tam = 5;
	bool flag = false;
	string nomes[] = { "Maria", "Ana Carolina", "Davi", "Fernando", "Renato", "Gloria", "Erico", "Lais", "Juliano", "Rodrigo" };
	string sobrenomes[] = { "Moraes", "da Silva", "Pereira", "dos Santos", "Cruz", "Souza", "Lima", "Soares", "Zacarias", "Oliveira" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 10;
		if (i == 0) {
			lista_autores[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual de nomes
			}
			else if (flag == false) {
				lista_autores[i].nome = nomes[a]; //cadastra o nome 
				aux[i] = a;
			}
		}
	}
	int aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % 10;
		if (i == 0) {
			lista_autores[i].sobrenome = sobrenomes[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o sobrenome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual de sobrenomes
			}
			else if (flag == false) {
				lista_autores[i].sobrenome = sobrenomes[b]; //cadastra o sobrenome
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de editoras
void gerar_editoras(Editora lista_editoras[]) {
	const int tam = 9;
	bool flag = false;
	string nomes[] = { "Editora Plano", "Liberdade", "Recanto Editora", "Editora Arcanjo", "Hiperlivros", "Cometa", "Monumental", "Ponto&Virgula", "Sabedoria", "AlfaLetras", "SempreLer", "MundoPerdido" };
	string locais[] = { "Brasilia", "Porto Alegre", "Sao Paulo", "Rio de Janeiro", "Florianopolis", "Salvador", "Curitiba", "Santos", "Itajai", "Joinville", "Manaus", "Vitoria", "Belo Horizonte", "Palmas" };

	int aux[tam] = { 0 }, a = 0, aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 12;
		if (i == 0) {
			lista_editoras[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome de editora ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_editoras[i].nome = nomes[a];
				aux[i] = a;
			}
		}
	}
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % 14;
		if (i == 0) {
			lista_editoras[i].local = locais[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o local ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_editoras[i].local = locais[b];
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de assuntos
void gerar_assuntos(string lista_assuntos[]) {
	const int tam = 9;
	string assuntos[] = { "Comedia", "Terror", "Romance", "Drama", "Ficcao Cientifica", "Suspense", "Horror", "Romance de Epoca", "Historia Real", "Historia", "Comedia Romantica", "Autoajuda", "Terror Psicologico", "Fantasia", "Religiao", "Poesia" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 16;
		if (i == 0) {
			lista_assuntos[i] = assuntos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			bool flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o assunto ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_assuntos[i] = assuntos[a];
				aux[i] = a;
			}
		}
	}
}

//gera cadastros iniciais de anos de publicacao
void gerar_ano(int lista_ano[]) {
	int aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 150;
		lista_ano[i] = 2023 - aux; //sorteia ano de publicação aleatorio
	}
}

//gera cadastros iniciais de mes de publicacao
void gerar_mes(int lista_mes[]) {
	int v[10] = { 0 }, aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 12 + 1;
		if (i == 0) {
			lista_mes[i] = aux;
			v[i] = aux;
		}
		else if (i > 0) {
			if (aux != v[i - 1]) {
				lista_mes[i] = aux;
				v[i] = aux;
			}
			else {
				i--;
			}
		}
	}
}

// gera cadastros iniciais de livros
void gerar_livros(Livro& a, Editora b, Autor c, string assunto, string titulo, int ano, int id) {
	a.titulo = titulo;
	a.autor = c;
	a.editora = b;
	a.publicacao = ano;
	a.assunto = assunto;
	a.id = id;
}

void gerar_revistas(Revista& r, Editora b, string assunto, int ano, int mes, string titulo, int id) {
	r.titulo = titulo;
	r.editora = b;
	r.publicacao.mes = mes;
	r.publicacao.ano = ano;
	r.assunto = assunto;
	r.id = id;
}

void gerar_nomes(string lista_nomes[]) {
	const int tam = 3;
	string nomes[] = { "Pablo Jeremias", "Bernardo Junior", "Livia Luz", "Suzana Cruz", "Renata Silva", "Osvaldo Moreira", "Poliana Silva", "Julia Fernandes", "Cesar Neto" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 9;
		if (i == 0) {
			lista_nomes[i] = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			bool flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_nomes[i] = nomes[a];
				aux[i] = a;
			}
		}
	}
}

void gerar_usuarios(Usuario& u, int id, string nome) {
	u.nome = nome;
	u.id = id;
}

#pragma endregion Funcoes

#pragma region Alocacao de Memoria

template<typename T>
T* aumentarVetor(T* lista, int tam, int pos) {
	T* aux = new T[tam + pos];
	for (int i = 0; i < tam; i++) {
		aux[i] = lista[i];
	}
	delete[]lista;
	return aux;
}

//EU QUERIA FAZER UMA FUNÇÃO PRA VERIFICAR E ATUALIZAR AO MESMO TEMPO MAS NÃO ROLOU

//template<typename T>
//void verificador_espaco_ocupado(T* lista, int cont, int &mais, int TAM) {
//	if (cont == TAM - 1 + (mais - 1) * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
//		lista = aumentarVetor(lista, TAM + (mais - 1) * 5, 5 * mais); 
//		mais++; //indicador de quantas vezes o vetor foi aumentado
//	}
//}

void deletar(Livro* lista_livros, Revista* lista_revistas, Usuario* lista_usuarios, Editora* lista_editoras, Autor* lista_autores,
	string* lista_assuntos, string* lista_titulos, string* titulos_revistas, string* lista_nomes) { // Liberando a memória alocada para os vetores

	delete[]lista_autores;
	delete[]lista_editoras;
	delete[]lista_assuntos;
	delete[]lista_titulos;
	delete[]titulos_revistas;
	delete[]lista_nomes;
	delete[]lista_livros;
	delete[]lista_revistas;
	delete[]lista_usuarios;
}

#pragma endregion Alocacao de Memoria

#pragma region Menu
void exibir_menu_principal() {
	set_color(7);
	cout << "\n\n [1] - Cadastro\n"
		<< " [2] - Alteracao\n"
		<< " [3] - Retirada\n"
		<< " [4] - Consultar Acervo\n"
		<< " [5] - Central do Usuario\n"
		<< " [0] - Sair\n";
}

void submenu_cadastro() {
	set_color(6);
	cout << "\n - - - - - CADASTRO - - - - -\n" << endl;
	set_color(7);
	cout << " [1] - Livro\n"
		<< " [2] - Revista\n"
		<< " [3] - Autor\n"
		<< " [4] - Editora\n"
		<< " [0] - Voltar\n";
}

void submenu_alteracao() {
	set_color(6);
	cout << "\n - - - - - ALTERACAO - - - - -\n" << endl;
	set_color(7);
	cout << " [1] - Livro\n"
		<< " [2] - Revista\n"
		<< " [3] - Autor\n"
		<< " [4] - Editora\n"
		<< " [0] - Voltar\n";
}

void submenu_retirada() {
	set_color(6);
	cout << "\n - - - - - RETIRADA - - - - -\n" << endl;
	set_color(7);
	cout << " [1] - Livro\n"
		<< " [2] - Revista\n"
		<< " [0] - Voltar\n";
}

void submenu_acervo() {
	set_color(6);
	cout << "\n - - - - - CONSULTAR ACERVO - - - - -\n" << endl;
	set_color(7);
	cout << " [1] - Pesquisa Filtrada\n"
		<< " [2] - Listagem Completa\n"
		<< " [0] - Voltar\n";
}

void submenu_central_usuario() {
	set_color(6);
	cout << "\n - - - - - CENTRAL DO USUARIO - - - - -\n" << endl;
	set_color(7);
	cout << " [1] - Cadastro de Usuario\n"
		<< " [2] - Usuarios com atraso de devolucao\n"
		<< " [3] - Devolver livros e revistas\n"
		<< " [4] - Historico de retiradas\n"
		<< " [5] - Listagem de Usuarios\n"
		<< " [0] - Voltar\n";
}

void submenu_filtrar_pesquisa() {
	set_color(6);
	cout << "\nDeseja filtrar sua pesquisa por: " << endl;
	set_color(7);
	cout << "\n [1] - Livros\n"
		<< " [2] - Disponibilidade\n"
		<< " [3] - Revistas\n"
		<< " [4] - Autor\n"
		<< " [5] - Editora\n"
		<< " [6] - Assunto\n"
		<< " [0] - Voltar\n";
}
#pragma endregion  Menu

#define TAM 10

int main()
{
#pragma region Data atual
	int year, month, day;
	struct tm timeinfo;
	time_t now = time(0);

	localtime_s(&timeinfo, &now);
	year = timeinfo.tm_year + 1900;
	month = timeinfo.tm_mon + 1;
	day = timeinfo.tm_mday;

	Data data_atual = { day, month, year };

#pragma endregion Data atual

#pragma region listas
	//listas
	Autor* lista_autores = new Autor[TAM];
	Editora* lista_editoras = new Editora[TAM];
	string* lista_assuntos = new string[TAM], * lista_titulos = new string[TAM], * titulos_revistas = new string[TAM], * lista_nomes = new string[TAM];
	Livro* lista_livros = new Livro[TAM];
	Revista* lista_revistas = new Revista[TAM];
	Usuario* lista_usuarios = new Usuario[TAM];
	int lista_ano[TAM], lista_mes[TAM];
#pragma endregion listas

#pragma region variaveis
	//outras variaveis
	int escolha;
	int cont_livros = 4, cont_revistas = 3, cont_autor = 4, cont_editora = 8, cont_assunto = 8, cont_usuario = 2, soma = 0;
	int mais_livros = 1, mais_rev = 1, mais_usuarios = 1, mais_ed = 1, mais_autor = 1, mais_assunto = 1;
	char resposta;
	bool flag = false, flag_aux = false;
	Livro livro;
	Autor autor;
	Editora ed;
	string aux;
	Usuario usuario;
	Revista rev;
	Data data;
#pragma endregion variaveis

#pragma region gerar
	//gerar IDs iniciais
	int ID_livro = gerar_ID(1001);
	int ID_revista = gerar_ID(2001);
	int ID_usuario = gerar_ID(3001);

	int id_livro = ID_livro;
	int id_revista = ID_revista;
	int id_usuario = ID_usuario;

	gerar_autores(lista_autores); //cadastro de 5 autores 
	gerar_editoras(lista_editoras); //cadastro de 9 editoras 
	gerar_assuntos(lista_assuntos); //cadastro de 15 assuntos 
	gerar_titulos(lista_titulos, titulos_revistas); //cadastro de 9 titulos
	gerar_ano(lista_ano); //cadastro de 10 anos aleatorios
	gerar_mes(lista_mes); //cadastro de 10 meses aleatorios
	gerar_nomes(lista_nomes); //cadastro de 3 nomes aleatorios

	for (int i = 0; i < 5; i++) { //cadastro de 5 livros aleatorios
		gerar_livros(lista_livros[i], lista_editoras[i], lista_autores[i], lista_assuntos[i], lista_titulos[i], lista_ano[i], id_livro);
		id_livro++;
	}

	for (int i = 0; i < 4; i++) { //cadastro de 4 revistas aleatorias
		gerar_revistas(lista_revistas[i], lista_editoras[i + 5], lista_assuntos[i + 5], lista_ano[i + 5], lista_mes[i], titulos_revistas[i], id_revista);
		id_revista++;
	}

	for (int i = 0; i < 3; i++) { //cadastro de 3 usuarios aleatorios
		gerar_usuarios(lista_usuarios[i], id_usuario, lista_nomes[i]);
		id_usuario++;
	}
#pragma endregion gerar

	int escolha_menu_principal = 0;
	int escolha_submenu = 0;

	while (true)
	{
		set_color(6);
		cout << "\n- - - - - - - - BIBLIOTECA - - - - - - - - -" << endl;
		cout << "\t\t";
		mostrar_data(data_atual);
		exibir_menu_principal();
		escolha_menu_principal = opcao_invalida("\nDigite uma opcao do menu", 0, 5);

		// continua com a opção que o usuario escolheu
		switch (escolha_menu_principal)
		{
		case 0: // fecha o programa
			system("cls");
			set_color(6);
			cout << "Volte sempre! " << endl;
			set_color(7);
			return 0;
			break;

		case 1: // cadastro de material
			system("cls");
			do {
				system("cls");
				submenu_cadastro();
				escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 4);
				system("cls");

				switch (escolha_submenu) { // Casos para cada opção do submenu 1

				case 0:
					system("cls");
					break;

				case 1: //cadastrar livro
					do {
						set_color(6);
						cout << "\n\tPreencha os dados abaixo: " << endl;
						set_color(7);

						cont_livros++;
						//if (cont_livros == TAM - 1 + (mais_livros - 1) * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
						//	lista_livros = aumentarVetor(lista_livros, TAM + (mais_livros - 1) * 5, 5 * mais_livros);
						//	mais_livros++; //indicador de quantas vezes o vetor foi aumentado
						//}

						lista_livros[cont_livros] = cadastrar_livro(lista_autores, cont_autor, lista_editoras, cont_editora, id_livro, data_atual);
						id_livro++;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
						}
						set_color(2);
						cout << "\nLivro cadastrado com sucesso!" << endl;
						set_color(7);
						mostrar_livro(lista_livros[cont_livros]); //mostra o livro cadastrado

						cout << "\nDeseja cadastrar mais um livro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S'); //loop continua enquanto o usuario desejar cadastrar mais livros
					system("cls");
					break;

				case 2: //cadastrar revistas
					do {
						set_color(6);
						cout << "\n\tPreencha os dados abaixo: " << endl;
						set_color(7);
						cont_revistas++;
						if (cont_revistas == TAM - 1 + mais_rev * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
							lista_revistas = aumentarVetor(lista_revistas, TAM + (mais_rev - 1) * 5, 5 * mais_rev);
							mais_rev++; //indicador de quantas vezes o vetor foi aumentado
						}

						lista_revistas[cont_revistas] = cadastrar_revista(lista_editoras, cont_editora, data_atual, id_revista);
						id_revista++;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
						}
						set_color(2);
						cout << "\nRevista cadastrada com sucesso!" << endl;
						set_color(7);
						mostrar_revista(lista_revistas[cont_revistas]); //mostra a revista cadastrada

						cout << "\nDeseja cadastrar mais uma revista (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S');  //continua enquanto for desejado cadastrar mais revistas

					system("cls");
					break;

				case 3: //cadastrar autor
					do {
						set_color(6);
						cout << "\nLista atual de autores cadastrados: \n" << endl;
						set_color(7);

						for (int i = 0; i < cont_autor + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_autor(lista_autores[i]); //mostra todos os autores cadastrados ate o momento
							cout << endl;
						}

						cont_autor++;
						if (cont_autor == TAM - 1 + (mais_autor - 1) * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
							lista_autores = aumentarVetor(lista_autores, TAM + (mais_autor - 1) * 5, 5 * mais_autor); 
							mais_autor++; //indicador de quantas vezes o vetor foi aumentado
						}

						cin.ignore();
						set_color(6);
						cout << "\nPreencha os dados abaixo: \n" << endl;
						set_color(7);
						autor = ler_autor();

						if (verificador_autor(cont_autor, lista_autores, autor) == false) {
							lista_autores[cont_autor].nome = autor.nome; //se nao tiver cadastro ainda, atualiza a lista de autores
							lista_autores[cont_autor].sobrenome = autor.sobrenome;

							set_color(2);
							cout << "\n\tAutor cadastrado com sucesso!" << endl;
							set_color(7);
						}
						cout << "\nDeseja realizar outro cadastro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S');

					system("cls");
					break;

				case 4: //cadastrar editora
					do {
						set_color(6);
						cout << "\nLista atual de editoras cadastradas: \n" << endl;
						set_color(7);

						for (int i = 0; i < cont_editora + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_editora(lista_editoras[i]); //mostra todas as editoras cadastradas ate o momento
							cout << endl;
						}

						cont_editora++;
						if (cont_editora == TAM - 1 + (mais_ed - 1) * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
							lista_editoras = aumentarVetor(lista_editoras, TAM + (mais_ed - 1) * 5, 5 * mais_ed);
							mais_ed++; //indicador de quantas vezes o vetor foi aumentado
						}

						cin.ignore();
						set_color(6);
						cout << "\nPreencha os dados abaixo: \n" << endl;
						set_color(7);

						ed = ler_editora();

						if (verificador_editora(cont_editora, lista_editoras, ed) == false) {
							lista_editoras[cont_editora].nome = ed.nome; //se nao tiver cadastro ainda, atualiza a lista de editoras
							lista_editoras[cont_editora].local = ed.local;

							set_color(2);
							cout << "\n\tEditora cadastrado com sucesso!" << endl;
							set_color(7);
						}
						cout << "\nDeseja realizar outro cadastro (s/n)? ";
						cin >> resposta;

					} while (resposta == 's' || resposta == 'S');
					system("cls");
					break;
				}
			} while (escolha_submenu != 0);
			system("cls");
			break;

		case 2: // alteracao de material
			system("cls");
			do {
				submenu_alteracao();
				escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 4);
				system("cls");

				switch (escolha_submenu) {
				case 0: //voltar para o menu
					system("cls");
					break;

				case 1://alterar livro
					set_color(6);
					cout << "\nLivros cadastrados atualmente (id e titulo): \n" << endl;
					set_color(7);

					for (int i = 0; i < cont_livros + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_livros[i].titulo << " (" << lista_livros[i].id << ")\n";
					}
					escolha = opcao_invalida("\nEscolha a opcao que deseja alterar", 1, cont_livros + 1);

					//a unica coisa que nao sera possivel alterar é o ID, pois segue um padrão
					lista_livros[escolha - 1] = cadastrar_livro(lista_autores, cont_autor, lista_editoras, cont_editora, lista_livros[escolha - 1].id, data_atual);

					set_color(2);
					cout << "\nLivro alterado com sucesso!\n\n" << endl;
					set_color(7);
					system("pause");
					mostrar_livro(lista_livros[escolha - 1]); //mostra o livro recadastrado

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
					}
					break;

				case 2: //alterar revista
					set_color(6);
					cout << "\nRevistas cadastrados atualmente (id e titulo): \n" << endl;
					set_color(7);
					for (int i = 0; i < cont_revistas + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_revistas[i].titulo << " (" << lista_revistas[i].id << ")\n";
					}
					escolha = opcao_invalida("\nEscolha a opcao que deseja alterar", 1, cont_revistas + 1);

					//a unica coisa que nao sera possivel alterar é o ID, pois segue um padrão
					lista_revistas[escolha - 1] = cadastrar_revista(lista_editoras, cont_editora, data_atual, lista_revistas[escolha - 1].id);

					set_color(2);
					cout << "\n\tRevista alterada com sucesso!\n\n" << endl;
					set_color(7);
					system("pause");
					mostrar_revista(lista_revistas[escolha - 1]); //mostra a revista recadastrada

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
					}
					break;

				case 3: //alterar autor
					set_color(6);
					cout << "\nLista atual de autores cadastrados: \n" << endl;
					set_color(7);
					for (int i = 0; i < cont_autor + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						mostrar_autor(lista_autores[i]);
					}
					escolha = opcao_invalida("\nEscolha a opcao que deseja alterar", 1, cont_autor + 1);

					cin.ignore();
					autor = ler_autor();

					if (verificador_autor(cont_autor, lista_autores, autor) == false) { //verifica se ja nao eh um autor cadastrado
						lista_autores[escolha - 1] = autor;
						set_color(2);
						cout << "\n\tAutor alterado com sucesso!\n\n" << endl;
						set_color(7);

						//atualiza lista de livros tambem
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].autor.nome == autor.nome && lista_livros[i].autor.sobrenome == autor.sobrenome) {
								lista_livros[i].autor.nome = lista_autores[escolha - 1].nome;
								lista_livros[i].autor.sobrenome = lista_autores[escolha - 1].sobrenome;
							}
						}
						system("pause");
					}
					break;

				case 4: //alterar editora
					set_color(6);
					cout << "\nLista atual de editoras cadastradas: \n" << endl;
					set_color(7);
					for (int i = 0; i < cont_editora + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						mostrar_editora(lista_editoras[i]);
					}
					escolha = opcao_invalida("\nEscolha a opcao que deseja alterar", 1, cont_editora + 1);

					cin.ignore();
					ed = ler_editora();

					if (verificador_editora(cont_editora, lista_editoras, ed) == false) {
						lista_editoras[escolha - 1] = ed;
						set_color(2);
						cout << "\n\tEditora alterada com sucesso!\n\n" << endl;
						set_color(7);

						for (int i = 0; i < cont_livros + 1; i++) { //atualiza lista de livros com a editora alterada
							if (lista_livros[i].editora.nome == ed.nome && lista_livros[i].editora.local == ed.local) {
								lista_livros[i].editora.nome = lista_editoras[escolha - 1].nome;
								lista_livros[i].editora.local = lista_editoras[escolha - 1].local;
							}
						}
						for (int i = 0; i < cont_revistas + 1; i++) { //atualiza lista de revistas com a editora alterada
							if (lista_revistas[i].editora.nome == ed.nome && lista_revistas[i].editora.local == ed.local) {
								lista_revistas[i].editora.nome = lista_editoras[escolha - 1].nome;
								lista_revistas[i].editora.local = lista_editoras[escolha - 1].local;
							}
						}
						system("pause");
					}
					break;
				}
				system("cls");
			} while (escolha_submenu != 0);
			system("cls");
			break;

		case 3: // retirada
			system("cls");
			do {
				submenu_retirada();
				escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 2);
				system("cls");

				switch (escolha_submenu)
				{
				case 0:
					system("cls");
					break;

				case 1: // RETIRADA LIVROS
					system("cls");
					set_color(6);
					cout << "\nLista de Livros Disponiveis (Id - Titulo)\n" << endl;
					set_color(7);

					for (int i = 0; i < cont_livros + 1; i++) {
						if (lista_livros[i].disponivel) {
							cout << "\t[" << lista_livros[i].id << "] - ";
							cout << lista_livros[i].titulo << endl;
						}
					}

					escolha = opcao_invalida("\nInforme o ID do livro", ID_livro, id_livro - 1);

					do {
						cout << "\nEntre com a data de retirada:\n";
						data = ler_data();
					} while (data.dia > dias_no_mes(data.mes, data.ano));

					usuario.id = opcao_invalida("\nInforme seu ID de usuario", ID_usuario, id_usuario - 1);

					// Permitir a retirada do livro
					soma = 0;
					flag = false;

					for (int i = 0; i < cont_usuario + 1; i++) {
						if (lista_usuarios[i].id == usuario.id) {
							flag = true;

							if (atraso(lista_usuarios[i]) == false) {
								soma = qtd_retiradas(lista_usuarios[i]);
								lista_usuarios[i].retirados[soma].id = escolha;
								lista_usuarios[i].retirados[soma].retirada = data;
								lista_usuarios[i].retirados[soma].devolucao = data_devolucao(data);

								if (dias_passados(data, data_atual) > 7) {
									lista_usuarios[i].retirados[soma].atraso = true;
								}

								for (int j = 0; j < cont_livros + 1; j++) {
									if (lista_livros[j].id == escolha) {
										lista_livros[j].disponivel = false;
										break;
									}
								}

								set_color(2);
								cout << "\nItem retirado com sucesso!" << endl;
								set_color(7);
								cout << "\tVoce tem 7 dias para realizar a leitura do material" << endl;
								cout << "\tData de devolucao: ";
								mostrar_data(lista_usuarios[i].retirados[soma].devolucao);
								cout << endl << endl;
							}
							else {
								break;
							}
						}
					}
					if (flag == false) {
						set_color(4);
						cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou o ID corretamente!" << endl;
						set_color(7);
					}

					system("pause");
					system("cls");
					break;

				case 2: // RETIRADA REVISTAS
					system("cls");
					set_color(6);
					cout << "\nLista de Revistas Disponiveis (ID e titulo): \n" << endl;
					set_color(7);

					for (int i = 0; i < cont_revistas + 1; i++) {
						if (lista_revistas[i].disponivel) {
							cout << "\t[" << lista_revistas[i].id << "] ";
							cout << lista_revistas[i].titulo << endl;
						}
					}
					do {
						escolha = opcao_invalida("\nInforme o ID da revista que deseja retirar", ID_revista, id_revista - 1);

						flag = false;
						for (int i = 0; i < cont_revistas + 1; i++) {
							if (lista_revistas[i].id == escolha && lista_revistas[i].disponivel) {
								flag = true;
								break;
							}
						}
						if (!flag) {
							set_color(4);
							cout << "\nRevista indisponivel para retirada." << endl;
							set_color(7);
							system("pause");
							system("cls");
							break;
						}
					} while (flag == false);

					do {
						cout << "\nEntre com a data de retirada:\n";
						data = ler_data();
					} while (data.dia > dias_no_mes(data.mes, data.ano));

					usuario.id = opcao_invalida("\nInforme seu ID de usuario", ID_usuario, id_usuario - 1);

					soma = 0;
					flag = false;
					for (int i = 0; i < cont_usuario + 1; i++) {
						if (lista_usuarios[i].id == usuario.id) { //encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
							flag = true;

							if (atraso(lista_usuarios[i]) == false) {
								soma = qtd_retiradas(lista_usuarios[i]);
								lista_usuarios[i].retirados[soma].id = escolha; //atualiza lista de retirados do usuario
								lista_usuarios[i].retirados[soma].retirada = data;
								lista_usuarios[i].retirados[soma].devolucao = data_devolucao(data);

								if (dias_passados(data, data_atual) > 7) {
									lista_usuarios[i].retirados[soma].atraso = true;
								}

								for (int i = 0; i < cont_revistas + 1; i++) {
									if (lista_revistas[i].id == escolha) {
										lista_revistas[i].disponivel = false; //passa a colocar a revista como indisponivel
										break;
									}
								}
								set_color(2);
								cout << "\nItem retirado com sucesso!" << endl;
								set_color(7);
								cout << "\tVoce tem 7 dias para realizar a leitura do material" << endl;
								cout << "\tData de devolucao: ";
								mostrar_data(lista_usuarios[i].retirados[soma].devolucao);
								cout << endl << endl;
							}
							else if (atraso(lista_usuarios[i])) {
								break;
							}
						}
					}
					if (flag == false) {
						set_color(4);
						cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou a ID corretamente!" << endl;
						set_color(7);
					}
					system("pause");
					system("cls");
					break;
				}
			} while (escolha_submenu != 0);
			system("cls");
			break;

		case 4: // consultar acervo
			system("cls");
			do {
				submenu_acervo();
				escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 2);
				system("cls");

				switch (escolha_submenu) {
				case 0:
					system("cls");
					break;

				case 1:
					do {
						submenu_filtrar_pesquisa();
						escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 6);
						system("cls");

						switch (escolha_submenu) {
						case 0: //voltar para o menu
							system("cls");
							break;

						case 1: //mostrar todos os livros
							set_color(6);
							cout << "\n - - - - - LISTAGEM DE LIVROS - - - - -" << endl;
							set_color(7);
							for (int i = 0; i < cont_livros; i++) {
								mostrar_livro(lista_livros[i]);
							}
							system("pause");
							system("cls");
							break;

						case 2: // mostrar todos os livros e revistas disponiveis
							set_color(6);
							cout << "\nLista de itens disponiveis:\n" << endl;
							set_color(7);
							for (int i = 0; i < cont_livros + 1; i++) {
								if (lista_livros[i].disponivel) {
									mostrar_livro(lista_livros[i]);

								}
							}
							for (int i = 0; i < cont_revistas + 1; i++) {
								if (lista_revistas[i].disponivel) {
									mostrar_revista(lista_revistas[i]);
								}
							}
							system("pause");
							system("cls");
							break;

						case 3: //mostrar todas as revistas
							set_color(6);
							cout << "\n - - - - - LISTAGEM DE REVISTAS - - - - -" << endl;
							set_color(7);
							for (int i = 0; i < cont_revistas + 1; i++) {
								mostrar_revista(lista_revistas[i]);
							}
							system("pause");
							system("cls");
							break;

						case 4: //mostrar livros de um autor especifico
							set_color(6);
							cout << "\nLista atual de autores cadastrados: " << endl;
							set_color(7);
							for (int i = 0; i < cont_autor + 1; i++) {
								cout << "\t[" << i + 1 << "] ";
								mostrar_autor(lista_autores[i]);
							}
							cout << endl;
							cin.ignore();
							autor = ler_autor();

							flag = false;
							for (int i = 0; i < cont_livros + 1; i++) {
								if (lista_livros[i].autor.nome == autor.nome && lista_livros[i].autor.sobrenome == autor.sobrenome) { //verifica livros com o autor procurado
									mostrar_livro(lista_livros[i]);
									flag = true;
								}
							}
							if (flag == false) {
								set_color(4);
								cout << "\nNao existem livros cadastrados com este autor" << endl;
								set_color(7);
							}
							system("pause");
							system("cls");
							break;

						case 5: //mostrar livros e revistas de uma editora
							set_color(6);
							cout << "\nLista atual de editoras cadastradas: " << endl;
							set_color(7);
							for (int i = 0; i < cont_editora + 1; i++) {
								cout << "\t[" << i + 1 << "] ";
								mostrar_editora(lista_editoras[i]);
							}
							cout << endl;
							cin.ignore();
							ed = ler_editora();

							flag = false;
							for (int i = 0; i < cont_livros + 1; i++) {
								if (lista_livros[i].editora.nome == ed.nome && lista_livros[i].editora.local == ed.local) { //verifica se existem livros da editora procurada
									mostrar_livro(lista_livros[i]);
									flag = true;
								}
								else if (lista_revistas[i].editora.nome == ed.nome && lista_revistas[i].editora.local == ed.local) { //verifica as revistas
									mostrar_revista(lista_revistas[i]);
									flag = true;
								}
							}
							if (flag == false) {
								set_color(4);
								cout << "\nNao existem livros ou revistas cadastrados com esta Editora" << endl;
								set_color(7);
							}
							system("pause");
							system("cls");
							break;

						case 6: //mostrar livros e revistas de um assunto
							set_color(6);
							cout << "\nLista de Asssuntos cadastrados: \n" << endl;
							set_color(7);
							for (int i = 0; i < cont_assunto + 1; i++) {
								cout << "\t" << lista_assuntos[i] << endl;
							}

							cin.ignore();
							set_color(6);
							cout << "\nQual assunto deseja encontrar? ";
							set_color(7);
							getline(cin, aux);

							flag = false;
							for (int i = 0; i < cont_livros + 1; i++) {
								if (lista_livros[i].assunto == aux) { //verifica se existem livros relacionados ao assunto procurado
									mostrar_livro(lista_livros[i]);
									flag = true;
								}
								else if (lista_revistas[i].assunto == aux) { //verifica as revistas
									mostrar_revista(lista_revistas[i]);
									flag = true;
								}
							}
							if (flag == false) {
								set_color(4);
								cout << "\nNao existem livros ou revistas cadastrados com este assunto" << endl;
								set_color(7);
							}
							system("pause");
							system("cls");
							break;
						}
					} while (escolha_submenu != 0);
					system("cls");
					break;

				case 2:
					cout << "\n - - - - - LISTAGEM COMPLETA - - - - -" << endl;
					set_color(7);
					for (int i = 0; i < cont_livros + 1; i++) {
						mostrar_livro(lista_livros[i]);
					}
					for (int i = 0; i < cont_revistas + 1; i++) {
						mostrar_revista(lista_revistas[i]);
					}
					cout << endl;
					system("pause");
					system("cls");
					break;
				}

			} while (escolha_submenu != 0);
			system("cls");
			break;

		case 5: //informacoes relacionadas ao usuario
			system("cls");
			do {
				submenu_central_usuario();
				escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 5);
				system("cls");

				switch (escolha_submenu)
				{
				case 0:
					system("cls");
					break;

				case 1:
					system("cls");
					set_color(6);
					cout << "\n - - - - - CADASTRE-SE - - - - -\n" << endl;
					set_color(7);

					usuario = cadastrar_usuario();
					cont_usuario++;

					flag = false;
					for (int i = 0; i <= cont_usuario; i++) {
						if (lista_usuarios[i].nome == usuario.nome) { // verifica se o usuário já está cadastrado
							set_color(4);
							cout << "\n\tUsuario ja cadastrado.\n" << endl;
							set_color(7);
							cont_usuario--;
							flag = true;
							break;
						}
					}
					if (!flag) {
						if (cont_usuario == TAM - 1 + mais_usuarios * 5) { //verfica se o vetor esta completamente ocupado, se estiver, acrescenta 5 posições novas
							lista_usuarios = aumentarVetor(lista_usuarios, TAM + (mais_usuarios - 1) * 5, 5 * mais_usuarios);
							mais_usuarios++; //indicador de quantas vezes o vetor foi aumentado
						}

						lista_usuarios[cont_usuario].nome = usuario.nome;
						lista_usuarios[cont_usuario].id = id_usuario;
						set_color(2);
						cout << "\n\tUsuario cadastrado com sucesso!" << endl;
						set_color(7);
						cout << "\tSeu ID de usuario eh: " << lista_usuarios[cont_usuario].id << "\n\n";
						id_usuario++;
					}
					system("pause");
					system("cls");
					break;

				case 2:
					system("cls");
					set_color(6);
					cout << "- - - - - USUARIOS COM ATRASO DE DEVOLUCAO - - - - -\n" << endl;
					set_color(7);

					flag = false;
					for (int i = 0; i < cont_usuario + 1; i++) {
						for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
							if (lista_usuarios[i].retirados[j].atraso) {
								cout << "ID do Usuario: " << lista_usuarios[i].id << endl; //mostra ID do usuario q tem item em atraso
								flag = true;

								for (int k = 0; k < cont_livros + 1; k++) {
									if (lista_usuarios[i].retirados[j].id == lista_livros[k].id) { // mostra livros em atraso
										mostrar_livro(lista_livros[k]);
										cout << "\tData de retirada: ";
										mostrar_data(lista_usuarios[i].retirados[j].retirada);
										cout << "\tData para devolucao: ";
										mostrar_data(lista_usuarios[i].retirados[j].devolucao);
										cout << endl;
										break;
									}
								}
								for (int k = 0; k < cont_revistas + 1; k++) {
									if (lista_usuarios[i].retirados[j].id == lista_revistas[k].id) { // mostra revistas em atraso
										mostrar_revista(lista_revistas[k]);
										cout << "\tData de retirada: ";
										mostrar_data(lista_usuarios[i].retirados[j].retirada);
										cout << "\tData para devolucao: ";
										mostrar_data(lista_usuarios[i].retirados[j].devolucao);
										cout << endl;
										break;
									}
								}
								cout << endl;
							}
						}
					}
					if (!flag) {
						cout << "\nNenhum Usuario com status de atraso foi encontrado\n" << endl;
					}
					system("pause");
					system("cls");
					break;

				case 3:
					system("cls");
					set_color(6);
					cout << "- - - - - - - - - - - - - DEVOLUCAO - - - - - - - - - - - - - \n" << endl;
					set_color(7);
					cout << "[1] - Devolver livro\t"
						<< "[2] - Devolver revista\t"
						<< "  [0] - Sair\n";
					escolha_submenu = opcao_invalida("\nDigite uma opcao", 0, 2);
					system("cls");

					if (escolha_submenu == 1) {

						usuario.id = opcao_invalida("\nInforme ID de Usuario", ID_usuario, id_usuario - 1);

						flag = false;
						for (int i = 0; i < cont_usuario + 1; i++) {
							if (lista_usuarios[i].id == usuario.id) {

								cout << "\nLista de Retirados: " << endl;
								for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
									for (int k = 0; k < cont_livros + 1; k++) {
										if (lista_usuarios[i].retirados[j].id == lista_livros[k].id && lista_livros[k].disponivel == false) { // mostra livros retirados no momento
											cout << "\t[" << lista_usuarios[i].retirados[j].id << "] - ";
											cout << lista_livros[k].titulo << endl;
											cout << "\t\tData de retirada: ";
											mostrar_data(lista_usuarios[i].retirados[j].retirada);
											cout << "\n\t\tData para devolucao: ";
											mostrar_data(lista_usuarios[i].retirados[j].devolucao);
											cout << endl;
											flag_aux = true; //confirma que existem livros retirados no momento
										}
									}
								}
								break;
							}
						}
						if (!flag_aux) {
							cout << "\n\tUsuario nao possui nenhum livro retirado no momento\n" << endl;
							system("pause");
							system("cls");
							break; //se nao tiver livro retirado no momento, volta para o menu
						}

						livro.id = opcao_invalida("\nInforme o ID do livro a ser devolvido", ID_livro, id_livro - 1);

						for (int k = 0; k < cont_livros + 1; k++) {
							if (lista_livros[k].id == livro.id) { //verifica qual o livro que havia sido retirado e atualiza a disponibilidade
								lista_livros[k].disponivel = true;
								break;
							}
						}

						flag = false;
						for (int i = 0; i < cont_usuario + 1; i++) {
							for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
								if (lista_usuarios[i].retirados[j].id == livro.id) { //encontra o usuario que retirou o livro
									flag = true;
									cout << "\n\tData de retorno do livro: ";
									mostrar_data(data_atual);

									if (lista_usuarios[i].retirados[j].atraso) { //se tiver atraso mostra a quantidade de dias de atraso
										cout << "\n\tItem devolvido com atraso!" << endl;
										set_color(4);
										cout << "\tDias de atraso: " << dias_passados(lista_usuarios[i].retirados[j].retirada, data_atual) - 7 << "\n" << endl;
										set_color(7);
										system("pause");
										system("cls");
										break;
									}
									else {
										set_color(2);
										cout << "\nItem devolvido com sucesso!" << "\n" << endl;
										set_color(7);
										system("pause");
										break;
									}
								}
							}
						}
						if (!flag) {
							set_color(4);
							cout << "\nLivro nao encontrado em sua lista de retirados!\n" << endl;
							set_color(7);
							system("pause");
							break;
						}
						system("cls");
					}

					if (escolha_submenu == 2) { //devolver revista

						usuario.id = opcao_invalida("\nInforme ID de Usuario", ID_usuario, id_usuario - 1);

						flag = false;
						for (int i = 0; i < cont_usuario + 1; i++) {
							if (lista_usuarios[i].id == usuario.id) { // encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
								flag = true;
								cout << "\nLista de Retirados: " << endl;

								for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
									for (int k = 0; k < cont_revistas + 1; k++) {
										if (lista_usuarios[i].retirados[j].id == lista_revistas[k].id) { //mostra revistas retiradas
											cout << "\t[" << lista_usuarios[i].retirados[j].id << "] - ";
											cout << lista_revistas[k].titulo << endl;
											cout << "\t\tData de retirada: ";
											mostrar_data(lista_usuarios[i].retirados[j].retirada);
											cout << "\n\t\tData para devolucao: ";
											mostrar_data(lista_usuarios[i].retirados[j].devolucao);
											cout << endl;
											flag_aux = true; //confirma que existem livros retirados no momento
										}
									}
								}
							}
						}
						if (flag == false) { //caso nao seja um id valido
							set_color(4);
							cout << "\nID de Usuario nao foi encontrado, verifique existe cadastro ou se digitou o ID corretamente\n" << endl;
							set_color(7);
							system("pause");
							system("cls");
							break;
						}
						if (!flag_aux && flag) {
							cout << "\n\tUsuario nao possui nenhuma revista retirada no momento\n" << endl;
							system("pause");
							system("cls");
							break; //se nao tiver revista retirada no momento, volta para o menu
						}

						rev.id = opcao_invalida("\nInforme o ID da revista a ser devolvida: ", ID_revista, id_revista - 1);

						for (int k = 0; k < cont_revistas + 1; k++) {
							if (lista_revistas[k].id == rev.id) { //verifica qual a revista que havia sido retirada e atualiza a disponibilidade
								lista_revistas[k].disponivel = true;
								break;
							}
						}

						flag = false;
						for (int i = 0; i < cont_usuario + 1; i++) {
							for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
								if (lista_usuarios[i].retirados[j].id == rev.id) { //encontra o usuario que fez a retirada
									flag = true;
									cout << "\n\tData de retorno da revista: ";
									mostrar_data(data_atual);

									if (lista_usuarios[i].retirados[j].atraso) { //se tiver atraso mostra a quantidade de dias de atraso
										cout << "\n\tItem devolvido com atraso!" << endl;
										set_color(4);
										cout << "\tDias de atraso: " << dias_passados(lista_usuarios[i].retirados[j].retirada, data_atual) - 7 << endl;
										set_color(7);
										system("pause");
										system("cls");
										break;
									}
									else {
										set_color(2);
										cout << "\n\tItem devolvido com sucesso!\n" << endl;
										set_color(7);
										system("pause");
										break;
									}
								}
							}
						}
						if (!flag) {
							set_color(4);
							cout << "\nRevista nao encontrada em sua lista de retirados!\n" << endl;
							set_color(7);
							system("pause");
							break;
						}
						system("cls");
					}
					break;

				case 4:
					system("cls");
					set_color(6);
					cout << "- - - - - HISTORICO DE RETIRADA - - - - -\n" << endl;
					set_color(7);
					usuario.id = opcao_invalida("\nInforme o ID do usuario", ID_usuario, id_usuario - 1);

					flag_aux = false;

					for (int i = 0; i < cont_usuario + 1; i++) {
						if (lista_usuarios[i].id == usuario.id) { // encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
							cout << "\nHistorico de Retirada para " << lista_usuarios[i].nome << ": " << endl << endl;

							for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
								for (int k = 0; k < cont_livros + 1; k++) {
									if (lista_usuarios[i].retirados[j].id == lista_livros[k].id) { // mostra livros retirados
										mostrar_livro(lista_livros[k]);
										cout << "\tData de retirada: ";
										mostrar_data(lista_usuarios[i].retirados[j].retirada);
										cout << "\n\tData de devolucao: ";
										mostrar_data(lista_usuarios[i].retirados[j].devolucao);
										cout << endl;
										flag_aux = true; //confirma que houve pelo menos uma retirada
									}
								}
								for (int k = 0; k < cont_revistas + 1; k++) {
									if (lista_usuarios[i].retirados[j].id == lista_revistas[k].id) { //mostra revistas retiradas
										mostrar_revista(lista_revistas[k]);
										cout << "\tData de retirada: ";
										mostrar_data(lista_usuarios[i].retirados[j].retirada);
										cout << "\n\tData de devolucao: ";
										mostrar_data(lista_usuarios[i].retirados[j].devolucao);
										cout << endl;
										flag_aux = true; //confirma que houve pelo menos uma retirada
									}
								}
							}
						}
					}

					if (!flag_aux) {
						cout << "\tUsuario ainda nao realizou retirada de material da Biblioteca\n" << endl;
						system("pause");
						system("cls");
						break; //se nao tiver nenhuma retirada, volta para o menu
					}
					cout << endl;
					system("pause");
					system("cls");
					break;

				case 5: // Listagem de Usuarios
					system("cls");
					set_color(6);
					cout << "\t- - - - - LISTAGEM DE USUARIOS - - - - -" << endl;
					set_color(7);
					for (int i = 0; i < cont_usuario + 1; i++) {
						mostrar_usuario(lista_usuarios[i]);
					}
					cout << endl;
					system("pause");
					system("cls");
					break;
				}
			} while (escolha_submenu != 0);
			system("cls");
			break;
		}
	}
	deletar(lista_livros, lista_revistas, lista_usuarios, lista_editoras, lista_autores, lista_assuntos, lista_titulos, titulos_revistas, lista_nomes); //função que deleta todos os vetores
	return 0;
}