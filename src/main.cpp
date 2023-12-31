#include <unistd.h>
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "biblioteca.h"
#include "funcionario.h"
#include "funcoes.h"
//#include "conio.h"


#ifdef _WIN32
    // Código específico para Windows
    #define CLEAR_SCREEN "cls"
#elif __linux__
    // Código específico para Linux
    #define CLEAR_SCREEN "clear"
#endif

int main() {
    char click;
    string instrucoes = "\033[33mUse \"w\" para subir, \"s\" para descer e \"d\" para escolher.\n";
    string divisao = "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\033[0m";
    Biblioteca *b = new Biblioteca();
    vector<string> cabecalho = {instrucoes, divisao ,"Seja bem-vindo ao sistema de biblioteca avancado, para comecar basta entrar na sua conta, caso nao possua cadastro clique em \"Cadastrar\"."};
    vector<string> opcoes = {"Entrar.", "Cadastrar."};
    inicio:
    b->atualizar_geral();
    system(CLEAR_SCREEN);
    try {
        string resposta = menu2(opcoes, cabecalho);
        if (resposta == "Entrar.") {
            string id, senha;
            cout << "Id: ";
            cin >> id;
            cout << "Senha: ";
            cin >> senha;
            if (b->get_usuario(id) == nullptr && b->get_funcionario(id) == nullptr) {
                throw runtime_error("\033[1;31mIdentificacao nao exite.\033[0m");
            }
            if ((b->get_usuario(id) != nullptr && senha != b->get_usuario(id)->get_senha()) or (b->get_funcionario(id) != nullptr &&senha != b->get_funcionario(id)->get_senha())) {  /// tratamento de excessao para caso n encontre o usuario n de erro no nullptr->get_senha.
                throw invalid_argument("\033[1;31mA senha inserida esta errada! Tente novamente.\033[0m");
            }
            if (b->get_usuario(id) != nullptr) { /// se encontrar um usuario com esse id
                if (id.size() == 7 or id.size() == 9) {   /// aluno/prof
                    if (senha == b->get_usuario(id)->get_senha()) {
                        b->get_usuario(id)->ler_avisos(b);
                        vector<string> cabecalho = {instrucoes, divisao ,"O que deseja fazer?"};
                        vector<string> opcoes = {"Pagar multa.", "Ver minhas informacoes.", "Solicitar reserva livro.","Cancelar reserva.", "Obter link livro online.", "Solicitar renovacao emprestimo.","Sair."};
                        escolha_usuario:
                        try{
                            system(CLEAR_SCREEN);
                            string resposta = menu2(opcoes, cabecalho);
                            if (resposta == "Pagar multa.") {
                                b->get_usuario(id)->pagar_multa();

                            } else if (resposta == "Ver minhas informacoes.") {
                                b->get_usuario(id)->mostrar_informacoes_de_cadastro();

                            } else if (resposta == "Solicitar reserva livro.") {
                                b->get_usuario(id)->reservar_livro(b);

                            } else if (resposta == "Obter link livro online.") {
                                b->get_usuario(id)->retirar_livro_online(b);

                            } else if (resposta == "Cancelar reserva.") {
                                b->get_usuario(id)->cancelar_reserva();

                            } else if (resposta == "Solicitar renovacao emprestimo.") {
                                b->get_usuario(id)->renovar_emprestimo(b);

                            } else if (resposta == "Sair.") {
                                goto inicio;
                            }
                        }catch (const exception &e) {
                            cerr << e.what() << endl;
                            cin.clear();
                        }
                        cout << "aperte \"x\".";
                        click = cin.get();
                        goto escolha_usuario;
                    }
                }
            }
            if (b->get_funcionario(id) != nullptr) { /// se encontrar um funcionario com esse id.
                if (id.size() == 5) { ///func
                    if (senha == b->get_funcionario(id)->get_senha()) { /// se a senha pertence ao id colocado
                        vector<string> cabecalho = {instrucoes, divisao ,"O que deseja fazer?"};
                        vector<string> opcoes = {"Ver minhas informacoes.", "Ver informacoes de um usuario.", "Cadastrar livro.",
                                                 "Descadastrar livro.", "Ver informacoes livro.",
                                                 "Ver pedidos de reserva.",
                                                 "Ver pedidos de renovacao emprestimo.",
                                                 "Dar baixa emprestimo.", "Dar baixa devolucao.", "Sair."};
                        escolha_funcionario:
                        try {
                            system(CLEAR_SCREEN);
                            string resposta = menu2(opcoes, cabecalho);
                            if (resposta == "Ver minhas informacoes.") {
                                b->get_funcionario(id)->mostrar_informacoes_de_cadastro();

                            } else if (resposta == "Ver informacoes de um usuario.") {
                                b->get_funcionario(id)->mostrar_informacoes_de_cadastro(b);

                            } else if (resposta == "Cadastrar livro.") {
                                b->get_funcionario(id)->cadastrar_livro(b);


                            } else if (resposta == "Descadastrar livro.") {
                                b->get_funcionario(id)->descadastrar_livro(b);

                            } else if (resposta == "Ver informacoes livro.") {
                                b->ver_informacoes_livro();

                            } else if (resposta == "Ver pedidos de reserva.") {
                                b->get_funcionario(id)->dar_baixa_reserva(b->verificar_pedidos_reservas(), b);

                            } else if (resposta == "Ver pedidos de renovacao emprestimo.") {
                                b->get_funcionario(id)->dar_baixa_renovar_emprestimo(
                                        b->verificar_pedidos_renovar_emprestimo(),b);

                            } else if (resposta == "Dar baixa emprestimo.") {
                                b->get_funcionario(id)->entregar_livro_presencialmente(b);

                            } else if (resposta == "Dar baixa devolucao.") {
                                b->get_funcionario(id)->aceitar_devolucao_livro_presencialmente(b);

                            } else if (resposta == "Sair.") {
                                goto inicio;
                            }
                        }catch (const exception &e) {
                            cerr << e.what() << endl;
                            cin.clear();
                        }
                        cout << "aperte \"x\".";
                        click = cin.get();
                        goto escolha_funcionario;
                    }
                }
            }
        }if(resposta == "Cadastrar.") {    ///cadastrar
            cadastrar(b);
        }
    } catch (const exception &e) {
        cerr << "Atencao: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Aperte \"x\"";
    click = cin.get();
    goto inicio;
}


