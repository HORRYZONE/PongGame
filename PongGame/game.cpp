#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player1_p, player1_velocity;
float player2_p, player2_velocity;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_velocity_x = 100, ball_velocity_y, ball_half_size = 1;

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

internal void simulate_game(Input* input, float dt) {
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);

	
	float player1_acceleration = 0.f;
	//acceleration must be constant
	if (is_down(BUTTON_UP)) { player1_acceleration += 2000; }
	if (is_down(BUTTON_DOWN)) { player1_acceleration -= 2000; }


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
		}
		else if (ball_p_x - ball_half_size < -arena_half_size_x)
		{
			ball_velocity_x *= -1;
			ball_velocity_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
		}

	}

	//rendering
	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);
	draw_rect(80, player1_p, player_half_size_x, player_half_size_y, 0xff0000);
	draw_rect(-80, player2_p, player_half_size_x, player_half_size_y, 0xff0000);
}