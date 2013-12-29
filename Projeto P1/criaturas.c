#include "criaturas.h"

void preenche_criatura(Tcriatura *ser,float x,float y,float largura, float altura,int direcao,int f,int h,int r,int a,int pdf,
                       int at_ajusteX,int at_ajusteY,int at_largura,int at_altura,int quadro_at, int id_flecha)
{
    ser->x=x;
    ser->y=y;
    ser->altura = altura;
    ser->largura = largura;
    ser->at_ajusteX = at_ajusteX;
    ser->at_ajusteY = at_ajusteY;
    ser->at_largura = at_largura;
    ser->at_altura = at_altura;
    ser->quadro_at = quadro_at;
    ser->id_flecha = id_flecha;
    ser->direcao=direcao;
    ser->direcao_anterior=1;
    ser->estado_sprite=0;
    ser->caracteristicas.forca=f;
    ser->caracteristicas.habilidade=h;
    ser->caracteristicas.resistencia=r;
    ser->caracteristicas.armadura=a;
    ser->caracteristicas.poder_de_fogo=pdf;
    ser->caracteristicas.hp=ser->caracteristicas.resistencia*5;
    ser->caracteristicas.mp=ser->caracteristicas.resistencia*5;
    ser->caindo=0;
    ser->pulando=0;
    ser->permitir_pulo = 1;
    ser->atacando=0;
    ser->levando_dano=0;
    ser->alerta=0;
    ser->drop_item=0;
    ser->sprite = create_bitmap(64,64);
}

void imagens_guerreiro(Tcriatura *guerreiro)
{
    int i,j;
    BITMAP *tiles = load_bitmap(link_imagem("imagens_p1/guerreiro_.bmp"),NULL);
    BITMAP *tilesHp = load_bitmap(link_imagem("imagens_p1/barraHP.bmp"),NULL);

    for(i=0;i<2;i++)
    {
        for(j=0;j<4;j++)
        {
            guerreiro->vetor_sprite[j+4*i]=create_bitmap(64,64);
            blit(tiles,guerreiro->vetor_sprite[j+4*i],j*64,i*64,0,0,64,64);
        }
    }

    for(i=0;i<11;i++)
    {
        guerreiro->barraHp[i]=create_bitmap(123,16);
        blit(tilesHp,guerreiro->barraHp[i],0,i*16,0,0,123,16);
    }

    guerreiro->face = load_bitmap(link_imagem("imagens_p1/hero_face.bmp"),NULL);
    destroy_bitmap(tiles);
    destroy_bitmap(tilesHp);
}

void movimento_guerreiro(Tcriatura *guerreiro, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    if(!guerreiro->levando_dano) // se n�o esta levando dano
    {
        // para movimento s�o os estados 0 a 3 do sprite
        if(guerreiro->estado_sprite > 3 && !guerreiro->atacando)
            guerreiro->estado_sprite = 0;

        // apertou direita
        if(segurou(KEY_RIGHT))
            movimento_direita(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,0,3);

        // apertou esquerda
        if(segurou(KEY_LEFT))
            movimento_esquerda(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,0,3);

        // apertou para cima
        if(segurou(KEY_UP))
            pulo(guerreiro,guerreiro->caracteristicas.habilidade,0,matriz_tela,bloqueios);

        // apertou para baixo
        if(apertou(KEY_DOWN))
            guerreiro->tempo_recuo=timer; // limita tempo de recuo
        if(segurou(KEY_DOWN))
            recuo(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios);

        // se soltar o bot�o de pulo
        if(!key[KEY_UP])
        {
            guerreiro->pulando=0;
            guerreiro->caindo=1;
            guerreiro->permitir_pulo=1;
        }
    }
    else // se sofre dano
        recuo_por_dano(guerreiro,matriz_tela,bloqueios);

    // verifica��es b�sicas
    colide_chao(guerreiro,matriz_tela,bloqueios,1); // colidiu com o ch�o? primeira verifica��o obrigat�ria
    verificar_queda(guerreiro,matriz_tela,bloqueios); // atingiu o limite de pulo? est� em queda?
}

void verificar_status(Tcriatura *ser,Toponentes *inimigos, int tempo_jogo,Titens *itens)
{
    int i;
    if(ser->tempo_dano+20<=tempo_jogo)
        ser->levando_dano=0;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp <= 0 && !inimigos->goblins_guerreiros.goblins[i].drop_item)
        {
            inimigos->goblins_guerreiros.goblins[i].drop_item=1;
            deixa_item(itens,&inimigos->goblins_guerreiros.goblins[i]);
        }
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp <= 0 && !inimigos->goblins_arqueiros.goblins[i].drop_item)
        {
            inimigos->goblins_arqueiros.goblins[i].drop_item=1;
            deixa_item(itens,&inimigos->goblins_arqueiros.goblins[i]);
        }
    }
}

void ataque_guerreiro(Tcriatura *guerreiro,int tempo_jogo,Toponentes *inimigos)
{
    int i;
    int confirmacao=0;

    if(apertou(KEY_Z))
        confirmacao=1;

    ataque_ajustes(guerreiro,tempo_jogo,confirmacao,4,7);

    for(i=0;i<inimigos->goblins_guerreiros.n_goblins && guerreiro->atacando;i++)
    {
        if(!inimigos->goblins_guerreiros.goblins[i].levando_dano && inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp>0)
            ataque(guerreiro,&inimigos->goblins_guerreiros.goblins[i],tempo_jogo,0);
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins && guerreiro->atacando;i++)
    {
        if(!inimigos->goblins_arqueiros.goblins[i].levando_dano && inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp>0)
            ataque(guerreiro,&inimigos->goblins_arqueiros.goblins[i],tempo_jogo,0);
    }
    if(inimigos->chefes.chefe_atual!=0 && guerreiro->atacando)
    {
        if(!inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1].levando_dano &&
           inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1].caracteristicas.hp > 0)
            ataque(guerreiro,&inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1],tempo_jogo,0);
    }
}

void tocou_oponente(Tcriatura *guerreiro,Toponentes *inimigos,int tempo_jogo)
{
    int i;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->goblins_guerreiros.goblins[i].x,
            inimigos->goblins_guerreiros.goblins[i].y,inimigos->goblins_guerreiros.goblins[i].altura,
            inimigos->goblins_guerreiros.goblins[i].largura) && !guerreiro->levando_dano)
        {
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->goblins_guerreiros.goblins[i].x+inimigos->goblins_guerreiros.goblins[i].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->goblins_arqueiros.goblins[i].x,
            inimigos->goblins_arqueiros.goblins[i].y,inimigos->goblins_arqueiros.goblins[i].altura,
            inimigos->goblins_arqueiros.goblins[i].largura) && !guerreiro->levando_dano)
        {
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->goblins_arqueiros.goblins[i].x+inimigos->goblins_arqueiros.goblins[i].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].x,
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].y,inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].altura,
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].largura) && !guerreiro->levando_dano)
        {
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].x+inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }

    if(guerreiro->tempo_dano+20 <= tempo_jogo)
        guerreiro->levando_dano=0;
}

void desenhar_guerreiro(BITMAP *buffer,Tcriatura *guerreiro,int ajuste_x)
{
    rectfill(guerreiro->sprite,0,0,64,64,makecol(255,0,255));

    if(guerreiro->direcao==1)
        draw_sprite_ex(guerreiro->sprite,guerreiro->vetor_sprite[guerreiro->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    else
        draw_sprite_ex(guerreiro->sprite,guerreiro->vetor_sprite[guerreiro->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    draw_sprite(buffer, guerreiro->sprite, ajuste_x + guerreiro->x - (64 - guerreiro->largura)/2,
                guerreiro->y - (64 - guerreiro->altura)/2); // manda guerreiro para buffer

    if(guerreiro->caracteristicas.hp>10)
        guerreiro->caracteristicas.hp=10;

    int hpAtual = guerreiro->caracteristicas.hp;

    if(hpAtual>=0 && hpAtual<=10)
        draw_sprite(buffer, guerreiro->barraHp[hpAtual], 10, 10);
    else
        if(hpAtual<0) draw_sprite(buffer, guerreiro->barraHp[0], 10, 10);

}

void imagens_goblin_guerreiro(Tcriatura *goblin, int preenchida)
{
    int i;
    BITMAP *tiles = load_bitmap(link_imagem("imagens_p1/goblinG.bmp"),NULL);

    if(preenchida)
    {
        for(i=0;i<8;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<8;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }

    for(i=0;i<8;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);

    destroy_bitmap(tiles);
}

void movimento_goblin_guerreiro(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 5 && !goblin1->atacando)
            goblin1->estado_sprite = 5;
        if(goblin1->alerta)
        {
            if (goblin1->x-3 > guerreiro->x+guerreiro->largura || goblin1->x-4 > guerreiro->x+guerreiro->largura || goblin1->x-5 > guerreiro->x+guerreiro->largura
                || goblin1->x-6 > guerreiro->x+guerreiro->largura)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else if (goblin1->x+goblin1->largura+3 < guerreiro->x || goblin1->x+goblin1->largura+4 < guerreiro->x
                     || goblin1->x+goblin1->largura+5 < guerreiro->x || goblin1->x+goblin1->largura+6 < guerreiro->x)
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }
            else
                goblin1->direcao=0;

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->x - (guerreiro->x + guerreiro->largura) >= SCREEN_W)
               || (goblin1->x + goblin1->largura < guerreiro->x && guerreiro->x - (goblin1->x +goblin1->largura) >= SCREEN_W))
                    goblin1->alerta=0;
        }
        else
        {
            if((tempo_jogo/120)%2==0)
                goblin1->direcao=1;
            else
                goblin1->direcao=2;
            if (goblin1->direcao==2)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+10>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
            goblin1->levando_dano=0;
    }

    // verifica��es b�sicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o ch�o? primeira verifica��o obrigat�ria
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? est� em queda?
}

void ataque_goblin_guerreiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0)
    {
        goblin->caindo=1;
        goblin->pulando=0;
        if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y)
        {
            goblin->direcao=goblin->direcao_anterior;

            ataque_ajustes(goblin,tempo_jogo,1,2,4);
            if(!guerreiro->levando_dano && goblin->atacando)
                ataque(goblin,guerreiro,tempo_jogo,0);

            goblin->direcao=0;
        }
    }
}

void desenhar_goblin_guerreiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void imagens_goblin_arqueiro(Tcriatura *goblin, int preenchida)
{
    int i;
    BITMAP *tiles = load_bitmap(link_imagem("imagens_p1/goblinA.bmp"),NULL);

    if(preenchida)
    {
        for(i=0;i<7;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<7;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }

    for(i=0;i<7;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);

    destroy_bitmap(tiles);
}

void movimento_goblin_arqueiro(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo,
                                int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 4 && !goblin1->atacando)
            goblin1->estado_sprite = 4;
        if(goblin1->alerta)
        {
            if (goblin1->x > guerreiro->x+(guerreiro->largura/2) && goblin1->x - (guerreiro->x + (guerreiro->largura/2)) < 150)
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=2;
            }
            else if (goblin1->x < guerreiro->x+(guerreiro->largura/2) && (guerreiro->x + (guerreiro->largura/2)) - goblin1->x < 150)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=1;
            }
            else
                goblin1->direcao=0;

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->x - (guerreiro->x + guerreiro->largura) >= SCREEN_W)
               || (goblin1->x + goblin1->largura < guerreiro->x && guerreiro->x - (goblin1->x +goblin1->largura) >= SCREEN_W))
                    goblin1->alerta=0;
        }
        else
        {
            if((tempo_jogo/120)%2==0)
                goblin1->direcao=1;
            else
                goblin1->direcao=2;
            if (goblin1->direcao==2)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=2;
            }
            else
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=1;
            }

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+10>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
            goblin1->levando_dano=0;
    }

    // verifica��es b�sicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o ch�o? primeira verifica��o obrigat�ria
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? est� em queda?
}

void ataque_goblin_arqueiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,Titens *itens)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0)
    {
        goblin->caindo=1;
        goblin->pulando=0;
        if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y &&
           !itens->todosItens[goblin->id_flecha-1].ativo)
        {
            goblin->direcao=goblin->direcao_anterior;

            ataque_ajustes(goblin,tempo_jogo,1,2,3);
            if(goblin->tempo_ataque+10<=tempo_jogo)
            {
                itens->todosItens[goblin->id_flecha-1].direcao=goblin->direcao_anterior;
                itens->todosItens[goblin->id_flecha-1].ativo=1;
                itens->todosItens[goblin->id_flecha-1].x=goblin->x;
                itens->todosItens[goblin->id_flecha-1].y=goblin->y;
            }
            goblin->direcao=0;
        }
    }
}

void desenhar_goblin_arqueiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void imagens_goblin_chefe(Tcriatura *goblin, int preenchida)
{
    int i;
    BITMAP *tiles = load_bitmap(link_imagem("imagens_p1/goblinL.bmp"),NULL);

    if(preenchida)
    {
        for(i=0;i<8;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<8;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }
    for(i=0;i<8;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);

    destroy_bitmap(tiles);
}

void movimento_goblin_chefe(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 5 && !goblin1->atacando)
            goblin1->estado_sprite = 5;
        if(goblin1->alerta)
        {
            if (goblin1->x-3 > guerreiro->x+guerreiro->largura || goblin1->x-4 > guerreiro->x+guerreiro->largura || goblin1->x-5 > guerreiro->x+guerreiro->largura
                || goblin1->x-6 > guerreiro->x+guerreiro->largura)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else if (goblin1->x+goblin1->largura+3 < guerreiro->x || goblin1->x+goblin1->largura+4 < guerreiro->x
                     || goblin1->x+goblin1->largura+5 < guerreiro->x || goblin1->x+goblin1->largura+6 < guerreiro->x)
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }
            else
                goblin1->direcao=0;
        }
        else
        {
            if((tempo_jogo/120)%2==0)
                goblin1->direcao=1;
            else
                goblin1->direcao=2;
            if (goblin1->direcao==2)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+10>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
            goblin1->levando_dano=0;
    }

    // verifica��es b�sicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o ch�o? primeira verifica��o obrigat�ria
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? est� em queda?
}

void ataque_goblin_chefe(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0)
    {
        goblin->caindo=1;
        goblin->pulando=0;
        if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y)
        {
            goblin->direcao=goblin->direcao_anterior;

            ataque_ajustes(goblin,tempo_jogo,1,2,4);
            if(!guerreiro->levando_dano && goblin->atacando)
                ataque(goblin,guerreiro,tempo_jogo,0);

            goblin->direcao=0;
        }
    }
}

void desenhar_goblin_chefe(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void acoes_goblins(Toponentes *inimigos, Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                   Titens *itens)
{
    int i;

    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        movimento_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[i],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[i],guerreiro,tempo_jogo);
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        movimento_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[i],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[i],guerreiro,tempo_jogo,itens);
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        movimento_goblin_chefe(&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_chefe(&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],guerreiro,tempo_jogo);
    }
}

void desenhar_todos_goblins(Toponentes *inimigos,BITMAP *buffer, int ajuste_mapa)
{
    int i;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp<=0
                && !inimigos->goblins_guerreiros.goblins[i].levando_dano)
            inimigos->goblins_guerreiros.goblins[i].estado_sprite=0;
        desenhar_goblin_guerreiro(buffer,&inimigos->goblins_guerreiros.goblins[i],ajuste_mapa); // desenha goblin tipo 1 e manda para o buffer
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp<=0
                && !inimigos->goblins_arqueiros.goblins[i].levando_dano)
            inimigos->goblins_arqueiros.goblins[i].estado_sprite=0;
        desenhar_goblin_arqueiro(buffer,&inimigos->goblins_arqueiros.goblins[i],ajuste_mapa);
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].caracteristicas.hp<=0
                && !inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].levando_dano)
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].estado_sprite=0;
        desenhar_goblin_chefe(buffer,&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],ajuste_mapa);
    }
}

int dano_ataque(Tcriatura* atacante, int tipo_ataque)
{
    int ataque;
    if(tipo_ataque==0)
    {
        ataque=atacante->caracteristicas.forca;
    }
    else
    {
        ataque=atacante->caracteristicas.poder_de_fogo;
    }
    ataque+=atacante->caracteristicas.habilidade;
    float a = (rand()%100)/100.0;
    ataque= ataque*(0.8 + a*0.4);
    return ataque;
}


void calcular_dano(Tcriatura* atacante, Tcriatura* alvo,int tipo_ataque)
{
    int ataque = dano_ataque(atacante,tipo_ataque);
    //int forca=atacante->caracteristicas.forca;
    int defesa= alvo->caracteristicas.resistencia + alvo->caracteristicas.armadura;
    //int hp= alvo->caracteristicas.hp;
    float chance_critico=0.1;
    float a = (rand()%100)/100.0;
    int critico = chance_critico > a? 2 : 1;
    int dano;

    dano= (ataque-defesa)*critico;
    dano= dano > 0? dano : 1;

    alvo->caracteristicas.hp-=dano;
}

void movimento_direita(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup)
{
    int i;
    for(i=0; i < deslocamento && (ser->x + ser->largura) < LARGURA_MAPA &&
        !colisao_direita_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->x+=1;
    }
    if(mudar_direcao)
        ser->direcao=1;
    if(mudar_sprite)
    {
        if (!ser->pulando)
            mudanca_sprite(sprite_inf,sprite_sup,&ser->estado_sprite,12,0,0);
    }
}

void movimento_esquerda(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup)
{
    int i;
    for(i=0; i < deslocamento && ser->x > 0 &&
        !colisao_esquerda_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->x-=1;
    }
    if(mudar_direcao)
        ser->direcao=2;
    if(mudar_sprite)
    {
        if (!ser->pulando)
            mudanca_sprite(sprite_inf,sprite_sup,&ser->estado_sprite,12,0,0);
    }
}

void pulo(Tcriatura *ser,int deslocamentoy,int deslocamentox,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    int i;
    for(i=0; i < deslocamentoy && ser->y > ser->nivel_plataforma - ALTURA_PULO && !ser->caindo && ser->permitir_pulo &&
        !colisao_cima_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->pulando=1;
        ser->y-=1;
        if(deslocamentox!=0)
        {
            if(deslocamentox<0)
                movimento_esquerda(ser,(-1)*deslocamentox,matriz_tela,bloqueios,0,0,0,0);
            if(deslocamentox>0)
                movimento_direita(ser,deslocamentox,matriz_tela,bloqueios,0,0,0,0);
        }
    }
}

void recuo(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    if(ser->tempo_recuo + 20 >= timer && !ser->pulando && !ser->caindo)
    {
        if(ser->direcao==2)
            movimento_direita(ser,deslocamento*3,matriz_tela,bloqueios,0,0,0,0);
        else
            movimento_esquerda(ser,deslocamento*3,matriz_tela,bloqueios,0,0,0,0);
    }
}

void recuo_por_dano(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    ser->estado_sprite=0;
    ser->caindo=1;
    ser->pulando=0;

    // ele recua dependendo da dire��o que estiver
    if (ser->direcao==2)
        movimento_direita(ser,6,matriz_tela,bloqueios,0,0,0,0);
    else
        movimento_esquerda(ser,6,matriz_tela,bloqueios,0,0,0,0);
}

void mudanca_sprite(int limite_inferior,int limite_superior,int *estado_sprite,int intervalo,int tempo_inicio,int tempo_jogo)
{
    if(*estado_sprite < limite_inferior || *estado_sprite > limite_superior)
        *estado_sprite = limite_inferior;
    if (timer%intervalo==0 && tempo_inicio<=0)
    {
        *estado_sprite = *estado_sprite - limite_inferior;
        *estado_sprite = (*estado_sprite + 1) % (limite_superior-limite_inferior+1);
        *estado_sprite = *estado_sprite + limite_inferior;
    }
    if (tempo_inicio>0)
    {
        *estado_sprite = ((tempo_jogo-tempo_inicio)/intervalo)%(limite_superior-limite_inferior+1);
        *estado_sprite = *estado_sprite + (limite_superior-limite_inferior + 1);
    }
}

void colide_chao(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],int eh_heroi)
{
    if(colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios))
    {
        ser->caindo=0;
        if(eh_heroi==1)
        {
            if(!key[KEY_UP])ser->nivel_plataforma = ser->y+ser->altura - 1;
        }
        else
            ser->nivel_plataforma = ser->y+ser->altura - 1;
        ser->pulando=0;
        if(eh_heroi==1)
        {
            if(!key[KEY_UP])ser->permitir_pulo=1;
        }
        else
            ser->permitir_pulo=1;
    }
}

void verificar_queda(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    int i;

    if(ser->y <= ser->nivel_plataforma - ALTURA_PULO ||
       (!colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios)
        && !ser->pulando)||
       colisao_cima_mapa(ser->x,ser->y,ser->altura,ser->largura,matriz_tela,bloqueios))
    {
        ser->caindo=1;
        ser->permitir_pulo = 0;
    }

    for(i=0;i<5 && ser->caindo && !colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->y+=1;
    }
}

void ataque_ajustes(Tcriatura *atacante,int tempo_jogo,int confirmacao,int sprite_lim_inf,int sprite_lim_sup)
{
    if(!atacante->atacando && confirmacao)
    {
        atacante->atacando = 1;
        atacante->tempo_ataque = tempo_jogo;
    }
    if(atacante->tempo_ataque + (sprite_lim_sup-sprite_lim_inf+1)*5 <= tempo_jogo)
        atacante->atacando = 0;

    if(atacante->atacando)
        mudanca_sprite(sprite_lim_inf,sprite_lim_sup,&atacante->estado_sprite,5,atacante->tempo_ataque,tempo_jogo);
}

void ataque(Tcriatura *atacante,Tcriatura *alvo,int tempo_jogo,int tipo_at)
{
    if(atacante->atacando && !alvo->levando_dano)
    {
        if(atacante->direcao==2)//esquerda
        {
            if(colisao(atacante->x + atacante->at_ajusteX,atacante->y + atacante->at_ajusteY,atacante->at_altura,atacante->at_largura,
                       alvo->x,alvo->y,alvo->altura,alvo->largura)&& tempo_jogo-atacante->tempo_ataque > (atacante->quadro_at-1)*5)
               {
                   alvo->levando_dano=1;
                   alvo->tempo_dano=tempo_jogo;
                   alvo->direcao = 1;
                   calcular_dano(atacante,alvo,tipo_at);
               }
        }
        else //direita
        {
            if(colisao(atacante->x + atacante->largura + ((-1)*(atacante->at_largura+atacante->at_ajusteX)),atacante->y+atacante->at_ajusteY,atacante->at_altura,atacante->at_largura,
                       alvo->x,alvo->y,alvo->altura,alvo->largura )&& tempo_jogo-atacante->tempo_ataque > (atacante->quadro_at-1)*5)
               {
                   alvo->levando_dano=1;
                   alvo->tempo_dano=tempo_jogo;
                   alvo->direcao = 2;
                   calcular_dano(atacante,alvo,tipo_at);
               }
        }
    }
}

void deixa_item(Titens *itens,Tcriatura *goblin)
{
    int i,chance;
    for(i=0;i<itens->n_itens;i++)
    {
        if(!itens->todosItens[i].ativo && itens->todosItens[0].tipo==1)
        {
            chance=rand()%100;
            if(chance<=15)
            {
                itens->todosItens[i].x=goblin->x;
                itens->todosItens[i].y=goblin->y+goblin->altura-itens->todosItens[i].altura;
                itens->todosItens[i].ativo=1;
            }
        }
    }
}
