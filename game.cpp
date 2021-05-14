#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(1, 3);

float player_p_x, player_p_y, player_dp_x, player_dp_y;
float arena_half_size_x = 93, arena_half_size_y = 45;
float platform_p_x_1 = -30, platform_p_y_1 = -25, platform_half_size_x = 12, platform_half_size_y = 2.5;
float platform_p_x_2 = -40, platform_p_y_2 = 0;
float platform_p_x_3 = 40, platform_p_y_3 = 45;
float player_half_size_x = 2.5, player_half_size_y = 2.5;
float coin_p_x, coin_p_y, coin_half_size_x = 2, coin_half_size_y = 2;
bool coin_taken, platform_1_flip, platform_2_flip, platform_3_flip;
bool on_ground, on_platform_1, on_platform_2, on_platform_3;
int player_score, time_left = 1700, coin_platform = distr(gen);

internal void
simulate_player(float *p_x, float *p_y, float *dp_x, float *dp_y, float ddp_x, float ddp_y, float dt)
{
    ddp_x -= *dp_x * 10.f;
    ddp_y -= *dp_y * 10.f;

    *p_x = *p_x + *dp_x * dt + ddp_x * dt * dt * .5f;
    *dp_x = *dp_x + ddp_x * dt;

    *p_y = *p_y + *dp_y * dt + ddp_y * dt * dt * .5f;
    *dp_y = *dp_y + ddp_y * dt;

    if (*p_x + player_half_size_x > arena_half_size_x)
    {
        *p_x = arena_half_size_x - player_half_size_x;
        *dp_x *= 0;
    }

    else if (*p_x - player_half_size_x < -arena_half_size_x)
    {
        *p_x = -arena_half_size_x + player_half_size_x;
        *dp_x *= 0;
    }

    if (*p_y - player_half_size_y < -arena_half_size_y)
    {
        *p_y = -arena_half_size_y + player_half_size_y;
        *dp_y *= 0;
        on_ground = true;
        on_platform_1 = on_platform_2 = on_platform_3 = false;
    }

    if ((*p_y - player_half_size_y) < (platform_p_y_1 + platform_half_size_y) && (*p_y - player_half_size_y) > platform_p_y_1 && *p_x > (platform_p_x_1 - platform_half_size_x) && *p_x < (platform_p_x_1 + platform_half_size_x))
    {
        *p_y = platform_half_size_y + platform_p_y_1 + player_half_size_y;
        *dp_y *= 0;
        on_platform_1 = on_ground = true;
        on_platform_2 = on_platform_3 = false;
    }

    else if ((*p_y + player_half_size_y) > (platform_p_y_1 - platform_half_size_y) && (*p_y + player_half_size_y) < platform_p_y_1 && *p_x > (platform_p_x_1 - platform_half_size_x) && *p_x < (platform_p_x_1 + platform_half_size_x))
    {
        *p_y = -platform_half_size_y + platform_p_y_1 - player_half_size_y;
        *dp_y *= 0;
        on_platform_1 = on_platform_2 = on_platform_3 = false;
    }

    if ((*p_y - player_half_size_y) < (platform_p_y_2 + platform_half_size_y) && (*p_y - player_half_size_y) > platform_p_y_2 && *p_x > (platform_p_x_2 - platform_half_size_x) && *p_x < (platform_p_x_2 + platform_half_size_x))
    {
        *p_y = platform_half_size_y + platform_p_y_2 + player_half_size_y;
        *dp_y *= 0;
        on_platform_2 = on_ground = true;
        on_platform_1 = on_platform_3 = false;
    }

    else if ((*p_y + player_half_size_y) > (platform_p_y_2 - platform_half_size_y) && (*p_y + player_half_size_y) < platform_p_y_2 && *p_x > (platform_p_x_2 - platform_half_size_x) && *p_x < (platform_p_x_2 + platform_half_size_x))
    {
        *p_y = -platform_half_size_y + platform_p_y_2 - player_half_size_y;
        *dp_y *= 0;
        on_platform_1 = on_platform_2 = on_platform_3 = false;
    }

    if ((*p_y - player_half_size_y) < (platform_p_y_3 + platform_half_size_y) && (*p_y - player_half_size_y) > platform_p_y_3 && *p_x > (platform_p_x_3 - platform_half_size_x) && *p_x < (platform_p_x_3 + platform_half_size_x))
    {
        *p_y = platform_half_size_y + platform_p_y_3 + player_half_size_y;
        *dp_y *= 0;
        on_platform_3 = on_ground = true;
        on_platform_1 = on_platform_2 = false;
    }

    else if ((*p_y + player_half_size_y) > (platform_p_y_3 - platform_half_size_y) && (*p_y + player_half_size_y) < platform_p_y_3 && *p_x > (platform_p_x_3 - platform_half_size_x) && *p_x < (platform_p_x_3 + platform_half_size_x))
    {
        *p_y = -platform_half_size_y + platform_p_y_3 - player_half_size_y;
        *dp_y *= 0;
        on_platform_1 = on_platform_2 = on_platform_3 = false;
    }

    if (player_p_x - player_half_size_x < coin_p_x + coin_half_size_x && player_p_x + player_half_size_x > coin_p_x - coin_half_size_x && player_p_y - player_half_size_y < coin_p_y + coin_half_size_y && player_p_y + player_half_size_y > coin_p_y - coin_half_size_y)
    {
        player_score += 10;
        time_left = 1700;
        draw_rect(coin_p_x, coin_p_y, coin_half_size_x, coin_half_size_y, 0x000000);
        coin_platform = distr(gen);
    }
}

internal void
move_platforms()
{
    if (platform_p_x_1 - platform_half_size_x < -arena_half_size_x)
        platform_1_flip = true;

    else if (platform_p_x_1 + platform_half_size_x > arena_half_size_x)
        platform_1_flip = false;

    if (platform_p_x_2 - platform_half_size_x < -arena_half_size_x)
        platform_2_flip = false;

    else if (platform_p_x_2 + platform_half_size_x > arena_half_size_x)
        platform_2_flip = true;

    if (platform_p_y_3 - platform_half_size_y < -arena_half_size_y)
        platform_3_flip = true;

    else if (platform_p_y_3 + platform_half_size_y > arena_half_size_y)
        platform_3_flip = false;

    if (platform_1_flip)
    {
        platform_p_x_1 += .13f;

        if (on_platform_1)
            player_p_x += .13f;

        if (coin_platform == 1)
            coin_p_x += .13f;
    }

    else
    {
        platform_p_x_1 -= .13f;

        if (on_platform_1)
            player_p_x -= .13f;

        if (coin_platform == 1)
            coin_p_x -= .13f;
    }

    if (platform_2_flip)
    {
        platform_p_x_2 -= .13f;

        if (on_platform_2)
            player_p_x -= .13f;

        if (coin_platform == 2)
            coin_p_x -= .13f;
    }

    else
    {
        platform_p_x_2 += .13f;

        if (on_platform_2)
            player_p_x += .13f;

        if (coin_platform == 2)
            coin_p_x += .13f;
    }

    if (platform_3_flip)
    {
        platform_p_y_3 += .13f;

        if (on_platform_3)
            player_p_y += .13f;

        if (coin_platform == 3)
            coin_p_y += .13f;
    }

    else
    {
        platform_p_y_3 -= .13f;

        if (on_platform_3)
            player_p_y -= .13f;

        if (coin_platform == 3)
            coin_p_y -= .13f;
    }
}

enum Gamemode
{
    GM_MENU,
    GM_GAMEPLAY,
    GM_OVER,
    GM_QUIT,
};

Gamemode current_gamemode = GM_GAMEPLAY;
int hot_button, hot_button2;

internal void simulate_game(Input *input, float dt)
{

    if (current_gamemode == GM_GAMEPLAY)
    {

        if (coin_platform == 1)
        {
            coin_p_x = platform_p_x_1;
            coin_p_y = platform_p_y_1 + platform_half_size_y + 2;
        }

        else if (coin_platform == 2)
        {
            coin_p_x = platform_p_x_2;
            coin_p_y = platform_p_y_2 + platform_half_size_y + 2;
        }

        else
        {
            coin_p_x = platform_p_x_3;
            coin_p_y = platform_p_y_3 + platform_half_size_y + 2;
        }

        draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);
        draw_arena_borders_bottom(arena_half_size_x, arena_half_size_y, 0x079A22);
        draw_arena_borders_top(arena_half_size_x, arena_half_size_y, 0x000000);
        draw_rect(platform_p_x_1, platform_p_y_1, platform_half_size_x, platform_half_size_y, 0xaaaaaa);
        draw_rect(platform_p_x_2, platform_p_y_2, platform_half_size_x, platform_half_size_y, 0xaaaaaa);
        draw_rect(platform_p_x_3, platform_p_y_3, platform_half_size_x, platform_half_size_y, 0xaaaaaa);

        draw_rect(coin_p_x, coin_p_y, coin_half_size_x, coin_half_size_y, 0xffd700);
        draw_text("C", coin_p_x - .5f, coin_p_y + 1, .35f, 0xc72502);

        draw_number(player_score, 80, 40, 1.5, 0x2f658e);
        draw_number(time_left, 0, 40, 1.5, 0xffffff);

        float player_ddp_x = 0.f;
        float player_ddp_y = -1200.f;

        if (is_down(BUTTON_A))
            player_ddp_x -= 1000;

        if (is_down(BUTTON_D))
            player_ddp_x += 1000;

        if (is_down(BUTTON_SHIFT))
            player_ddp_x *= 2;

        if (pressed(BUTTON_SPACE) && on_ground)
        {
            player_ddp_y += 170000;
            on_ground = false;
        }

        simulate_player(&player_p_x, &player_p_y, &player_dp_x, &player_dp_y, player_ddp_x, player_ddp_y, dt);

        draw_rect(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xffffff);

        move_platforms();

        time_left--;

        if (time_left <= 0)
            current_gamemode = GM_OVER;

        // if (pressed(BUTTON_ESCAPE))
        // {
        //     ball_inc = 1;
        //     quit_prompt = true;
        //     current_gamemode = GM_QUIT;
        // }
    }

    else if (current_gamemode == GM_OVER)
    {
        draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);
        draw_arena_borders_bottom(arena_half_size_x, arena_half_size_y, 0x079A22);
        draw_arena_borders_top(arena_half_size_x, arena_half_size_y, 0x000000);
        draw_rect(platform_p_x_1, platform_p_y_1, platform_half_size_x, platform_half_size_y, 0xaaaaaa);
        draw_rect(platform_p_x_2, platform_p_y_2, platform_half_size_x, platform_half_size_y, 0xaaaaaa);
        draw_rect(platform_p_x_3, platform_p_y_3, platform_half_size_x, platform_half_size_y, 0xaaaaaa);

        draw_rect(coin_p_x, coin_p_y, coin_half_size_x, coin_half_size_y, 0xffd700);
        draw_text("C", coin_p_x - .5f, coin_p_y + 1, .35f, 0xc72502);

        draw_number(player_score, 80, 40, 1.5, 0x2f658e);
        draw_number(time_left, 0, 40, 1.5, 0xffffff);

        draw_rect(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xffffff);

        draw_rect(3, 3, 40, 20, 0xf30707);
        draw_text("GAME OVER", -25, 10, 1, 0xffffff);

        if (pressed(BUTTON_ENTER))
        {
            current_gamemode = GM_GAMEPLAY;
            player_p_x = player_p_y = player_dp_x = player_dp_y = 0;
            arena_half_size_x = 93;
            arena_half_size_y = 45;
            platform_p_x_1 = -30;
            platform_p_y_1 = -25;
            platform_half_size_x = 12;
            platform_half_size_y = 2.5;
            platform_p_x_2 = -40;
            platform_p_y_2 = 0;
            platform_p_x_3 = 40;
            platform_p_y_3 = 45;
            player_half_size_x = 2.5;
            player_half_size_y = 2.5;
            coin_p_x = platform_p_x_1;
            coin_p_y = platform_p_y_1 + platform_half_size_y + 2;
            coin_half_size_x = 2;
            coin_half_size_y = 2;
            coin_taken = platform_1_flip = platform_2_flip = platform_3_flip = false;
            on_ground = on_platform_1 = on_platform_2 = on_platform_3 = false;
            player_score = 0;
            time_left = 1700;
        }
    }

    // else
    // {
    //     if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
    //     {
    //         hot_button = !hot_button;
    //     }

    //     if (pressed(BUTTON_ENTER))
    //     {
    //         current_gamemode = GM_GAMEPLAY;
    //         enemy_is_ai = hot_button ? 0 : 1;
    //     }

    //     if (pressed(BUTTON_ESCAPE))
    //         current_gamemode = GM_QUIT;

    //     if (hot_button == 0)
    //     {
    //         draw_text("SINGLE PLAYER", -80, -10, 1, 0xff0000);
    //         draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
    //     }

    //     else
    //     {
    //         draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
    //         draw_text("MULTIPLAYER", 20, -10, 1, 0xff0000);
    //     }

    //     draw_text("PONG GAME", -50, 40, 2, 0xffffff);
    //     // draw_text("WATCH THE STEP BY STEP TUTORIAL ON", -73, 22, .75, 0xffffff);
    //     // draw_text("YOUTUBE.COM/DANZAIDAN", -73, 15, 1.22, 0xffffff);
    // }
}