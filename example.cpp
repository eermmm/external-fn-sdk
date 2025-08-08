#include "sdk.h"

	struct GameContext
	{
		uintptr_t uworld = 0;
		uintptr_t controller = 0;

		Fortnite::UWorld world{};
		Fortnite::UGameInstance gi{};
		Fortnite::AGameState gs{};
		Fortnite::ALocalPLayer local{};
		std::vector<Fortnite::APlayer> enemies{};
	};

	inline GameContext ctx; 

auto cache() -> void
{
	while (TRUE)
	{

		ctx.enemies.clear();

		ctx.world.cache();
		ctx.uworld = ctx.world.uworld();

		ctx.gi.cache(ctx.uworld);

		ctx.gs.cache(ctx.uworld);

		ctx.local.cache(ctx.gi.LocalPlayers());
		ctx.controller = ctx.local.LocalPlayerController();

		for (int i = 0; i < ctx.gs.PlayerCount(); i++)
		{
			Fortnite::APlayer player{};
			player.cache(ctx.gs.PlayerArray(), i);

			if (!player.IsValid()) continue;

			if (player.PlayerTeamID() == ctx.local.LocalTeamID()) continue;

			ctx.enemies.push_back(player);
		}
	}
