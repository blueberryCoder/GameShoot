//
// Created by blueberry on 2023/2/8.
//
#include "common.h"
#include "draw.h"
#include "stage.h"
#include "util.h"

static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *alienBulletTexture;
static SDL_Texture *background;
static SDL_Texture *explosionTexture;

static int enemySpawnTimer = 0;
static int stageResetTimer = FPS * 2;
static Star stars[MAX_STARS];

static backgroundX = 0;

static void resetStage(void);

static void fireAlienBullet(Entity *e);

static void doBullets(void);

static void clipPlayer(void);

static void initStarfield(void);

static void doBackground(void);
static void doStarfield(void);
static void doExplosions(void);
static void addExplosions(int x, int y, int num);

static void drawBackground(void);

static void drawStarfield(void);

static void drawDebris(void);

static void drawExplosions(void);


static void doBackground(void) {
    if(--backgroundX < -SCREEN_WIDTH) {
        backgroundX = 0;
    }
}

static void doStarfield(void) {
    int i ;
    for( i=0; i< MAX_STARS; i++) {
        stars[i].x -= stars[i].speed;
        if(stars[i].x < 0) {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}

static void doExplosions(void)
{
    Explosion *e, *prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next ; e != NULL ; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}

static void doDebris(void)
{
    Debris *d, *prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next ; d != NULL ; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;

        if (--d->life <= 0)
        {
            if (d == stage.debrisTail)
            {
                stage.debrisTail = prev;
            }

            prev->next = d->next;
            free(d);
            d = prev;
        }

        prev = d;
    }
}

static void addExplosions(int x, int y, int num)
{
    Explosion *e;
    int i;

    for (i = 0 ; i < num ; i++)
    {
        e = malloc(sizeof(Explosion));
        memset(e, 0, sizeof(Explosion));
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32) - (rand() % 32);
        e->y = y + (rand() % 32) - (rand() % 32);
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4)
        {
            case 0:
                e->r = 255;
                break;

            case 1:
                e->r = 255;
                e->g = 128;
                break;

            case 2:
                e->r = 255;
                e->g = 255;
                break;

            default:
                e->r = 255;
                e->g = 255;
                e->b = 255;
                break;
        }

        e->a = rand() % FPS * 3;
    }
}
static void addDebris(Entity *e)
{
    Debris *d;
    int x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0 ; y <= h ; y += h)
    {
        for (x = 0 ; x <= w ; x += w)
        {
            d = malloc(sizeof(Debris));
            memset(d, 0, sizeof(Debris));
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS * 2;
            d->texture = e->texture;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}

static void drawBackground(void)
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX ; x < SCREEN_WIDTH ; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

static void drawStarfield(void)
{
    int i, c;

    for (i = 0 ; i < MAX_STARS ; i++)
    {
        c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}

static void drawDebris(void) {
    Debris *d;
    for(d = stage.debrisHead.next ; d != NULL; d = d->next) {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}

static void drawExplosions(void)
{
    Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next ; e != NULL ; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}


/**
 *
 * @param e  alien fly
 */
static void fireAlienBullet(Entity *e) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));

    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);
    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);
    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;
    e->reload = (rand() % FPS * 2);
}


static void spawnEnemies(void) {
    Entity *enemy;

    if (--enemySpawnTimer <= 0) {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        enemy->side = SIDE_ALIEN;
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->health = 1;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        enemy->dx = -(2 + (rand() % 4));
        enemySpawnTimer = 30 + (rand() % 60);
        enemy->reload = FPS * (1 + rand() % 3);
    }
}

static void doFighters(void) {
    Entity *e, *prev;
    prev = &stage.fighterHead;
    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;
        if (e != player && e->x < -e->w) {
            e->health = 0;
        }
        if (e->health == 0) {
            if (e == player) {
                player = NULL;
            }
            if (e == stage.fighterTail) {
                stage.fighterTail = prev;
            }
            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }

}

static void initPlayer() {
    player = (Entity *) malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture(PLAY_RES);
    player->side = SIDE_PLAYER;
    player->health = 1;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void fireBullet(void) {
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->side = SIDE_PLAYER;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);

    player->reload = 8;
}

static int bulletHitFighter(Entity *b) {
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h)) {
            b->health = 0;
            e->health = 0;
            // TODO
            addExplosions(e->x, e->y, 1);
            addDebris(e);
            return 1;
        }
    }
    return 0;
}

static void doBullets(void) {
    Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL; b = b->next) {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT) {
            if (b == stage.bulletTail) {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}

static void doPlayer(void) {
    if (player != NULL) {
        player->dx = player->dy = 0;

        if (player->reload > 0) {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP]) {
            player->dy = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_DOWN]) {
            player->dy = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LEFT]) {
            player->dx = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_RIGHT]) {
            player->dx = PLAYER_SPEED;
        }

        // LCTRL
        if (app.keyboard[SDL_SCANCODE_A] && player->reload == 0) {
            fireBullet();
        }

        player->x += player->dx;
        player->y += player->dy;
    }
}

static void doEnemies(void) {
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        if (e != player && player != NULL && --e->reload <= 0) {
            fireAlienBullet(e);
        }
    }
}

static void logic(void) {
    doBackground();
    doStarfield();

    doPlayer();
    clipPlayer();
    doFighters();
    doBullets();

    spawnEnemies();
    doEnemies();

    doExplosions();
    doDebris();

    if (player == NULL && --stageResetTimer <= 0) {
        resetStage();
    }
}

static void drawFighters(void) {
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        blit(e->texture, e->x, e->y);
    }
}

static void drawBullets(void) {
    Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next) {
        blit(b->texture, b->x, b->y);
    }
}

static void draw(void) {
    drawBackground();
    drawStarfield();

    drawFighters();
    drawBullets();

    drawDebris();
    drawExplosions();
}

static void resetStage(void) {
    Entity *e;
    while (stage.fighterHead.next) {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }
    while (stage.bulletHead.next) {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initStarfield();
    initPlayer();
    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
}

static void initStarfield(void)
{
    int i;

    for (i = 0 ; i < MAX_STARS ; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}

static void clipPlayer(void) {
    if (player != NULL) {
        if (player->x < 0) {
            player->x = 0;
        }
        if (player->y < 0) {
            player->y = 0;
        }
        if (player->x > SCREEN_WIDTH / 2) {
            player->x = SCREEN_WIDTH / 2;
        }
        if (player->y > SCREEN_HEIGHT - player->h) {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;


    alienBulletTexture = loadTexture(ALIEN_BULLET_RES);
    bulletTexture = loadTexture(BULLET_RES);
    enemyTexture = loadTexture(ENEMY_RES);
    background = loadTexture(BACKGROUND_RES);
    explosionTexture = loadTexture(EXPLOSION_RES);

    resetStage();
}