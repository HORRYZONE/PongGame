#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player1_p, player1_velocity;
float player2_p, player2_velocity;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_velocity_x = 130, ball_velocity_y, ball_half_size = 1;

int player_1_score, player_2_score;

internal void simulate_player(float *player_position, float *velocity, float acceleration, float dt) { 

	float friction = 10.f;

	//make some friction effects when stopping
	acceleration -= *velocity * friction;

	//player position(new) = player position(old) + (player velocity * dt) +  ((player acceleration * dt^2) /2 )
	*player_position = *player_position + *velocity * dt + acceleration * dt * dt * .5f;

	//player velocity(new) = player velocity(old) + (player acceleration * dt)
	*velocity = *velocity + acceleration * dt;

	//check for collisions with arena walls
	if (*player_position + player_half_size_y > arena_half_size_y)
	{
		*player_position = arena_half_size_y - player_half_size_y;
		*velocity = 0;

	}
	else if (*player_position - player_half_size_y < -arena_half_size_y)
	{
		*player_position = -arena_half_size_y + player_half_size_y;
		*velocity = 0;
	}

};

internal bool aaabb_collision(float x1, float y1, float half_size_x1, float half_size_y1,
	float x2, float y2, float half_size_x2, float half_size_y2) {
	if (x1 + half_size_x1 > x2 - half_size_x2 &&
		x1 - half_size_x1 < x2 + half_size_x2 &&
		y1 + half_size_y1 > y2 - half_size_y2 &&
		y1 - half_size_y1 < y2 + half_size_y2)
	{
		return true;
	}
	else
	{
		return false;
	}
};

enum Gamemode
{
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void simulate_game(Input* input, float dt) {
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xff5500);

	if (current_gamemode == GM_GAMEPLAY)
	{
		float player1_acceleration = 0.f;

		if (!enemy_is_ai)
		{
			//FOR MULTIPLAYER, TWO PLAYERS
			//acceleration must be constant
			if (is_down(BUTTON_UP)) { player1_acceleration += 2000; }
			if (is_down(BUTTON_DOWN)) { player1_acceleration -= 2000; }
		}
		else
		{
			// FOR AI, SINGLE PLAYER
			//if (ball_p_y > player1_p + 2.f) { player1_acceleration += 1300; }
			//if (ball_p_y < player1_p - 2.f) { player1_acceleration -= 1300; }
			player1_acceleration = (ball_p_y - player1_p) * 100;
			if (player1_acceleration > 1300) player1_acceleration = 1300;
			if (player1_acceleration < -1300) player1_acceleration = -1300;
		}

		float player2_acceleration = 0.f;
		//acceleration must be constant
		if (is_down(BUTTON_W)) { player2_acceleration += 2000; }
		if (is_down(BUTTON_S)) { player2_acceleration -= 2000; }

		simulate_player(&player1_p, &player1_velocity, player1_acceleration, dt);
		simulate_player(&player2_p, &player2_velocity, player2_acceleration, dt);

		//Simulate ball
		{
			ball_p_x = ball_p_x + ball_velocity_x * dt;
			ball_p_y = ball_p_y + ball_velocity_y * dt;

			if (aaabb_collision(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player1_p, player_half_size_x, player_half_size_y))
			{
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_velocity_x *= -1;
				//ball_velocity_y = player1_velocity * 0.75f;
				ball_velocity_y = (ball_p_y - player1_p) * 2 + player1_velocity * 0.75f;
			}

			else if (aaabb_collision(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player2_p, player_half_size_x, player_half_size_y))
			{
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_velocity_x *= -1;
				ball_velocity_y = (ball_p_y - player2_p) * 2 + player2_velocity * 0.75f;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y)
			{
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_velocity_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y)
			{
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_velocity_y *= -1;
			}
			else if (ball_p_x + ball_half_size > arena_half_size_x)
			{
				ball_velocity_x *= -1;
				ball_velocity_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x)
			{
				ball_velocity_x *= -1;
				ball_velocity_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;
			}

		}

		draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

		//rendering
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);
		draw_rect(80, player1_p, player_half_size_x, player_half_size_y, 0xff0000);
		draw_rect(-80, player2_p, player_half_size_x, player_half_size_y, 0xff0000);

	}

	else
	{
		// --- HELPER LAMBDA FOR DYNAMIC CENTERING ---
		auto draw_centered_text = [](const char* text, float y, float size, u32 color) {
			int len = 0;
			const char* ptr = text;
			while (*ptr++) len++;
			float x = -(len * 6.0f * size) * 0.5f;
			draw_text(text, x, y, size, color);
			};

		// Menu Input Handling (Left/Right for side-by-side options)
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
		{
			hot_button = !hot_button; // Toggle between 0 (SOLO) and 1 (DUO)
		}

		if (pressed(BUTTON_ENTER))
		{
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}

		// --- UI RENDERING ---

		// 1. Title ("PONG GAME") - Large and centered
		draw_centered_text("PONG GAME", 24.f, 2.0f, 0xffffff);

		// 2. Subtitle ("BY DAFT") - Moved lower (y = 10.f) to clear the title
		draw_text("BY DAFT", 32.f, 8.f, 0.4f, 0xaa55ff);

		// 3. Menu Options (Spread further apart: x = -24.f and x = 12.f)
		float menu_y = -4.f;
		float menu_size = 1.0f;
		u32 active_color = 0xffff00;   // Bright yellow highlight
		u32 dimmed_color = 0x555566;   // Distinct dark slate/gray

		if (hot_button == 0)
		{
			draw_text("SOLO", -24.f, menu_y, menu_size, active_color);
			draw_text("DUO", 12.f, menu_y, menu_size, dimmed_color);
		}
		else
		{
			draw_text("SOLO", -24.f, menu_y, menu_size, dimmed_color);
			draw_text("DUO", 12.f, menu_y, menu_size, active_color);
		}

		// 4. Instruction Hint - Made slightly smaller with a distinct cyan/blue color
		draw_centered_text("USE ARROWS AND ENTER", -30.f, 0.4f, 0xffffff);

		// 5. Footer / Credits - Small, gray, at the bottom
		draw_centered_text("SHOUT OUT TO DAN ZAIDAN FOR THE TUTORIALS", -36.f, 0.25f, 0x888888);
	}
	
}