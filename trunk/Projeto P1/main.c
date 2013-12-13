/*  #################################################################################
    #includes

    Inclua somente arquivos h do projeto
    As bibliotecas básicas são incluídas em básico.h
*/
#include "basico.h"
#include "mapa.h"
#include "criaturas.h"
#include "controle.h"
//  #################################################################################

int main()
{
    init(); // inicializa

/*  #######################################################################################
                    declaração das variáveis (e algumas inicializações)
    #######################################################################################
*/
    int i; // controlador de loops e auxiliares
    int ticks; // controla velocidade do jogo
    int tempo_de_jogo=0; // controla o tempo de jogo
    int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32]; // matriz da tela
    int mov_mapa[2]; // vetor que cuida do movimento do mapa e dos objetos nele
    mov_mapa[0]=0; // o primeiro índice controla com o segundo os objetos no mapa
    mov_mapa[1]=0;
    int controle_janela[2]; // controla tempo da janela atual, com tempo inicio e fim
    int janela_atual = 0; // controla janela atual
    int tela = 0, selecionar = 0; //controla o carregamento de cada tela
    int fase=1;
    int carrega_fase=1;
    char nome_fase[N_FASES][10]={"mapa1.txt","mapa2.txt","mapa3.txt"};

    //BITMAPs do Menu
    BITMAP *bg = load_bitmap(link_imagem("imagens_p1/menu.bmp"), NULL); //fundo do menu
    BITMAP *menu_iniciar = load_bitmap (link_imagem("imagens_p1/play.bmp"), NULL);//menu iniciar
    BITMAP *menu_options = load_bitmap (link_imagem("imagens_p1/options.bmp"), NULL);//menu opções
    BITMAP *menu_exit = load_bitmap (link_imagem("imagens_p1/exit.bmp"), NULL);//menu saída
    BITMAP *menu_iniciar_selecionado = load_bitmap (link_imagem("imagens_p1/play1.bmp"), NULL);//menu iniciar
    BITMAP *menu_options_selecionado = load_bitmap (link_imagem("imagens_p1/options1.bmp"), NULL);//menu opções
    BITMAP *menu_exit_selecionado = load_bitmap (link_imagem("imagens_p1/exit1.bmp"), NULL);

    // variáveis de objetos
    Tcriatura guerreiro; // declara objeto guerreiro
    Tcriatura goblin1; // declara objeto goblin

    // declara BITMAPS
    BITMAP *buffer = create_bitmap(SCREEN_W,SCREEN_H); // Cria o buffer;
    BITMAP *mapa = create_bitmap(LARGURA_MAPA,ALTURA_MAPA); // Cria o mapa
    BITMAP *texturas[MAX_TERRENOS]; // declara a array de texturas

    // Declara fontes
    FONT* corpo_texto = load_font("fontes/corpo.pcx",NULL,NULL);
    FONT* titulo_texto = load_font("fontes/titulos.pcx",NULL,NULL);



/*  #######################################################################################
                                fim da declaração das variáveis
    #######################################################################################
*/
    // carregamento inicial
    preenche_criatura(&guerreiro,0,NIVEL_CHAO-34,20,34,1,2,2,2,1,0); // preenche status guerreiro
    imagens_guerreiro(guerreiro.vetor_sprite); // preenche vetor de imagens do guerreiro
    preenche_criatura(&goblin1,SCREEN_W-50,NIVEL_CHAO-36,18,36,2,1,1,1,0,0); // preenche status goblin
    imagens_goblin1(goblin1.vetor_sprite); // preenche vetor de imagens do goblin tipo 1
    carrega_texturas(texturas); // prepara as texturas


    // configura saída com o botão x no alto da tela
    exit_program = FALSE;
    LOCK_FUNCTION(fecha_programa);
    LOCK_VARIABLE(exit_program);
    set_close_button_callback(fecha_programa); // fecha a qualquer momento com o botão fechar

    // timer
    timer=0;
    LOCK_FUNCTION(incrementa_timer);
    LOCK_VARIABLE(timer);
    install_int_ex(incrementa_timer,BPS_TO_TIMER(60)); // para que o jogo execute 60 vezes por segundo

    // inicializa controles de velocidade
    ticks = timer; // velocidade do jogo
    guerreiro.controle_estado = timer; // velocidade de modificação do sprite guerreiro
    goblin1.controle_estado = timer; // velocidade de modificação do sprite goblin1

    // Processo de repetição principal
    while (!exit_program)
    {
        // loop válido
        while(ticks<=timer && !exit_program)
        {
            // termina jogo se pressionar esc
            if(key[KEY_ESC])
                fecha_programa();

             if(tela==0)
            {
                clear_bitmap(buffer);
                keyboard_input();
                draw_sprite(buffer, bg, 0, 0);

                if(apertou(KEY_DOWN))
                {
                    selecionar = (selecionar+1)%3;
                }

                else if(apertou (KEY_UP))
                {
                    if (selecionar != 0)
                    selecionar = (selecionar-1)%3;

                    else if (selecionar == 0)
                    selecionar = 2;

                }

                if (selecionar == 0)
                {

                    draw_sprite (buffer, menu_iniciar_selecionado, 196, 166);
                    draw_sprite (buffer, menu_options, 196, 221);
                    draw_sprite (buffer, menu_exit, 196, 272);

                }

                else if (selecionar == 1)
                {
                    draw_sprite (buffer, menu_iniciar, 196, 166);
                    draw_sprite (buffer, menu_options_selecionado, 196, 221);
                    draw_sprite (buffer, menu_exit, 196, 272);
                }

                else if (selecionar == 2)
                {
                    draw_sprite (buffer, menu_iniciar, 196, 166);
                    draw_sprite (buffer, menu_options, 196, 221);
                    draw_sprite (buffer, menu_exit_selecionado, 196, 272);
                }
                draw_sprite (screen, buffer,0, 0);

                if (selecionar == 2 && apertou(KEY_ENTER))
                fecha_programa();

                else if (selecionar == 0 && apertou(KEY_ENTER))
                tela = 1;

            }

            else if(tela==1)
            {
                if(carrega_fase)
                {
                    prepara_mapa(&matriz_tela, nome_fase[fase-1]); // preenche matriz com os tilesets corretos
                    carrega_mapa(mapa,texturas,matriz_tela); // cria mapa com as texturas
                    carrega_fase=0;
                }

                // incrementa o tempo de jogo
                tempo_de_jogo++;

                // limpa bitmaps de armazenamento
                clear_bitmap(buffer); // Limpa o buffer;
                clear_bitmap(guerreiro.sprite); // Limpa bitmap guerreiro
                clear_bitmap(goblin1.sprite); // Limpa bitmap goblin tipo 1

                // atualiza estado do teclado
                keyboard_input();

                // Lógica do jogo
                movimento_guerreiro(&guerreiro,mov_mapa);
                goblin1.x += mov_mapa[0] - mov_mapa[1]; // ajusta posição goblin com mov_mapa
                movimento_goblin1(&goblin1,guerreiro.x);
                mov_mapa[1] = mov_mapa[0]; // evita acumulação no próximo ajuste mapa (se houver)

                if(apertou(KEY_W)) // W controla caixa de texto. teste.
                {
                    if(janela_atual==0)
                    {
                        janela_atual=1;
                        controle_janela[0]=tempo_de_jogo;
                        controle_janela[1]=-1;
                    }
                    else
                        controle_janela[1]=tempo_de_jogo+20;
                }

                // Desenhar
                draw_sprite(buffer, mapa, mov_mapa[0], 0); // manda mapa para o buffer na posição mov_mapa
                desenhar_goblin1(buffer,&goblin1); // desenha goblin tipo 1 e manda para o buffer
                desenhar_guerreiro(buffer,&guerreiro); // desenha guerreiro e manda para buffer
                janela_texto(buffer,SCREEN_W/2-60,10,120,50,"Kill Goblins","",
                             titulo_texto,corpo_texto,150,0,-1,tempo_de_jogo); // desenha titulo
                janela_variavel(buffer,SCREEN_W-50,0,50,50,(tempo_de_jogo)/60,titulo_texto,40); // desenha tempo

                if(janela_atual==1) // teste de janela
                {
                    janela_texto(buffer,150,250,300,100,"Jaques","Oi, esse e meu nome.",
                             titulo_texto,corpo_texto,150,controle_janela[0],
                             controle_janela[1],tempo_de_jogo); // exemplo caixa texto
                    if(tempo_de_jogo==controle_janela[1])
                        janela_atual=0;
                }
                if(colisao(guerreiro.x,guerreiro.y,guerreiro.altura,guerreiro.largura, goblin1.x, goblin1.y,goblin1.altura,goblin1.largura))
                rectfill(buffer,50,50,100,100,makecol(0,0,160));

                blit(buffer,screen,0,0,0,0,LARGURA_SCREEN,ALTURA_SCREEN); // Manda o buffer para a tela;

                // nova fase
                if(guerreiro.x +guerreiro.largura -mov_mapa[0] >= LARGURA_MAPA-50 && fase<N_FASES)
                {
                    carrega_fase=1;
                    guerreiro.x = 10;
                    goblin1.x = 300;
                    mov_mapa[0]=0;
                    mov_mapa[1]=0;
                    fase++;
                }

            }
            ticks++; // incrementa controle de velocidade do jogo
        }
    }

    deinit(); // finaliza

    // destruição de bitmaps
    destroy_bitmap(buffer);
    destroy_bitmap(mapa);
    destroy_bitmap(bg);
    destroy_bitmap(menu_iniciar);
    destroy_bitmap(menu_iniciar_selecionado);
    destroy_bitmap(menu_options);
    destroy_bitmap(menu_options_selecionado);
    destroy_bitmap(menu_exit);
    destroy_bitmap(menu_exit_selecionado);

    for(i=0;i<MAX_TERRENOS;i++)
    {
        destroy_bitmap(texturas[i]);
    }
    for(i=0;i<4;i++)
    {
        destroy_bitmap(guerreiro.vetor_sprite[i]);
    }
    for(i=0;i<3;i++)
    {
        destroy_bitmap(goblin1.vetor_sprite[i]);
    }
    destroy_bitmap(guerreiro.sprite);
    destroy_bitmap(goblin1.sprite);
    destroy_font(corpo_texto);
    destroy_font(titulo_texto);

    return 0 ;
}
END_OF_MAIN(); // para finalizar o main no allegro
