#include "eventos.h"

void verificar_evento(int *pause,int fase,Teventos *eventos,Tcriatura *guerreiro,Tjanelas *janelas)
{
    // eventos da primeira fase
    if(fase==1)
    {
        // evento 1
        if(!eventos->eventos_executados[0] &&
           (guerreiro->x>=100 || eventos->evento_atual==1))
        {
            // trava o evento atual e pausa o jogo
            if(!eventos->evento_atual)
                eventos->evento_atual=1;

            if(*pause==0)
                *pause=1;

            // agora chama janela com texto
            if(janelas->janela_atual==0 && janelas->total[1].tempo_fim==-1)
            {
                janelas->janela_atual=1;
                janelas->total[0].tempo_inicio=timer;
                janelas->total[0].tempo_fim=-1;
            }

            if(apertou(KEY_ENTER)||apertou(KEY_SPACE))
            {
                if(janelas->janela_atual==1)
                {
                    janelas->janela_atual=2;
                    janelas->total[1].tempo_inicio=timer;
                    janelas->total[1].tempo_fim=-1;
                }
                else if(janelas->janela_atual==2 && janelas->total[1].tempo_fim==-1)
                    janelas->total[1].tempo_fim=timer+20;
            }
            // agora a condi��o de sa�da � janelas->janela_atual==0
            if(janelas->janela_atual==0)
            {
                eventos->evento_atual=0;
                eventos->eventos_executados[0]=1;
                *pause=0;
            }
        }
    }
    else if(fase==2)
    {
        // evento 1
        if(!eventos->eventos_executados[0] &&
           (guerreiro->x>=100 || eventos->evento_atual==1))
        {
            // trava o evento atual e pausa o jogo
            if(!eventos->evento_atual)
                eventos->evento_atual=1;

            if(*pause==0)
                *pause=1;

            // agora chama janela com texto
            if(janelas->janela_atual==0 && janelas->total[0].tempo_fim==-1)
            {
                janelas->janela_atual=1;
                janelas->total[0].tempo_inicio=timer;
                janelas->total[0].tempo_fim=-1;
            }

            if(apertou(KEY_ENTER)||apertou(KEY_SPACE))
            {
                if(janelas->janela_atual==1 && janelas->total[0].tempo_fim==-1)
                    janelas->total[0].tempo_fim=timer+20;
            }
            // agora a condi��o de sa�da � janelas->janela_atual==0
            if(janelas->janela_atual==0)
            {
                eventos->evento_atual=0;
                eventos->eventos_executados[0]=1;
                *pause=0;
            }
        }
    }
    else if(fase==3)
    {
        // evento 1
        if(!eventos->eventos_executados[0] &&
           (guerreiro->x>=100 || eventos->evento_atual==1))
        {
            // trava o evento atual e pausa o jogo
            if(!eventos->evento_atual)
                eventos->evento_atual=1;

            if(*pause==0)
                *pause=1;

            // agora chama janela com texto
            if(janelas->janela_atual==0 && janelas->total[0].tempo_fim==-1)
            {
                janelas->janela_atual=1;
                janelas->total[0].tempo_inicio=timer;
                janelas->total[0].tempo_fim=-1;
            }

            if(apertou(KEY_ENTER)||apertou(KEY_SPACE))
            {
                if(janelas->janela_atual==1 && janelas->total[0].tempo_fim==-1)
                    janelas->total[0].tempo_fim=timer+20;
            }
            // agora a condi��o de sa�da � janelas->janela_atual==0
            if(janelas->janela_atual==0)
            {
                eventos->evento_atual=0;
                eventos->eventos_executados[0]=1;
                *pause=0;
            }
        }
    }
}

void desenhos_evento(BITMAP *buffer, int fase,Teventos *eventos,Tjanelas *janelas,Tcriatura *guerreiro,FONT* corpo_texto,
                     FONT *titulo_texto)
{
    // eventos da primeira fase
    if(fase==1)
    {
        // primeiro evento
        if(eventos->evento_atual==1)
        {
            if(janelas->janela_atual==1) // teste de janela
            {
                janela_dialogo(buffer,guerreiro,janelas->total[0].x,janelas->total[0].y,titulo_texto,corpo_texto,
                               janelas->total[0].tempo_inicio,janelas->total[0].tempo_fim,timer,janelas->total[0].titulo,
                               janelas->total[0].conteudo,1);
                if(timer==janelas->total[0].tempo_fim)
                    janelas->janela_atual=0;
            }
            else if(janelas->janela_atual==2)
            {
                janela_dialogo(buffer,guerreiro,janelas->total[1].x,janelas->total[1].y,titulo_texto,corpo_texto,
                               janelas->total[1].tempo_inicio,janelas->total[1].tempo_fim,timer,janelas->total[1].titulo,
                               janelas->total[1].conteudo,0);
                if(timer==janelas->total[1].tempo_fim)
                    janelas->janela_atual=0;
            }
        }
    }
    else if(fase==2)
    {
        // primeiro evento
        if(eventos->evento_atual==1)
        {
            if(janelas->janela_atual==1) // teste de janela
            {
                janela_dialogo(buffer,guerreiro,janelas->total[0].x,janelas->total[0].y,titulo_texto,corpo_texto,
                               janelas->total[0].tempo_inicio,janelas->total[0].tempo_fim,timer,janelas->total[0].titulo,
                               janelas->total[0].conteudo,1);
                if(timer==janelas->total[0].tempo_fim)
                    janelas->janela_atual=0;
            }
        }
    }
    else if(fase==3)
    {
        // primeiro evento
        if(eventos->evento_atual==1)
        {
            if(janelas->janela_atual==1) // teste de janela
            {
                janela_dialogo(buffer,guerreiro,janelas->total[0].x,janelas->total[0].y,titulo_texto,corpo_texto,
                               janelas->total[0].tempo_inicio,janelas->total[0].tempo_fim,timer,janelas->total[0].titulo,
                               janelas->total[0].conteudo,1);
                if(timer==janelas->total[0].tempo_fim)
                    janelas->janela_atual=0;
            }
        }
    }
}

void carregar_var_fase(int fase,Titens *itens, Tcriatura *guerreiro,Toponentes *inimigos,Tjanelas *janelas,BITMAP *background,
                       BITMAP *texturas[MAX_TERRENOS],Teventos *eventos)
{
    int i;
    BITMAP *fundo;

    for(i=0;i<3;i++)
        eventos->eventos_executados[i]=0;

    eventos->evento_atual=0;

    if(fase==1)
    {
        eventos->n_eventos=1;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,"imagens_p1/Itens1.bmp",6,12,1,1,0);
        itens->n_itens=1;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,2,3,2,1,0); // preenche status guerreiro
        imagens_guerreiro(guerreiro); // preenche vetor de imagens do guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],0); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],0); // preenche vetor de imagens do goblin tipo 1
        inimigos->goblins_guerreiros.n_goblins=2;

        carrega_texturas(texturas); // prepara as texturas

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Droga, me perdi nesta maldita floresta infestada de goblins.");
        preencher_janela(&janelas->total[1],70,300,0,0,0,0,-1,"Heroi","Agora tenho de sair daqui...");
        janelas->janela_atual=0;
        janelas->n_janelas=2;

        fundo = load_bitmap(link_imagem("imagens_p1/Forest01.bmp"),NULL);
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }

    if(fase==2)
    {
        eventos->n_eventos=1;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,"imagens_p1/Itens1.bmp",6,12,1,1,1);
        itens->n_itens=1;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,2,3,2,1,0); // preenche status guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],1); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],1); // preenche vetor de imagens do goblin tipo 1
        inimigos->goblins_guerreiros.n_goblins=2;

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Hum...");
        janelas->janela_atual=0;
        janelas->n_janelas=1;

        fundo = load_bitmap(link_imagem("imagens_p1/Stone2.bmp"),NULL);
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }

    if(fase==3)
    {
        eventos->n_eventos=1;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,"imagens_p1/Itens1.bmp",6,12,1,1,1);
        itens->n_itens=1;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,2,3,2,1,0); // preenche status guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],1); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],1); // preenche vetor de imagens do goblin tipo 1
        inimigos->goblins_guerreiros.n_goblins=2;

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Agora estou mais proximo de sair desse lugar horrivel.");
        janelas->janela_atual=0;
        janelas->n_janelas=1;

        fundo = load_bitmap(link_imagem("imagens_p1/Forest01.bmp"),NULL);
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }

    destroy_bitmap(fundo);
}

void pausar(int *pause)
{
    if (apertou(KEY_SPACE))
    {
        if (*pause == TRUE)
            *pause = FALSE;
        else
            *pause = TRUE;
    }
}

void verifica_nova_fase(Tcriatura *guerreiro, int *fase, int *carrega_fase, int *tela, int *loading_time, int *estagio_loading)
{
    if(guerreiro->x +guerreiro->largura >= LARGURA_MAPA-50 && *fase<N_FASES)
    {
        *carrega_fase=1;
        *fase=*fase+1;
        *tela=9; // a pr�xima tela ser� a de loading game
        *loading_time = timer;
        *estagio_loading=0;
    }
}
