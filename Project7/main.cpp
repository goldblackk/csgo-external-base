#include "functions.hpp"
#include "engine.hpp"

#include <chrono>
#include <thread>

c_engine g_engine;

//choke packets thread function
void chokepackets()
{
	while (true)
	{
		if (g_engine.is_ingame() && choke_packets)
		{
			g_engine.send_packets(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			g_engine.send_packets(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int main()
{
	//main startup function
	if (!startup()) {
		system("pause");
		return 0;
	}

	glow team, enemy;

	team.clr = color{ 0,1,0 };
	enemy.clr = color{ 1,0,0 };

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)chokepackets, 0, 0, 0);

	while (true)
	{
		c_base_entity local = g_engine.get_localplayer();
		glow_manager = rpm<DWORD>(client + signatures::dwGlowObjectManager);


		//choke packets toggle key
		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			choke_packets = !choke_packets;
		}


		//getting class id of an entity (you can use this to differentiate entity from entity)
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			system("cls");
			for (int i = 0; i <= 500; i++)
			{
				c_base_entity entity = g_engine.get_entity_from_index(i);
				if (entity.address)
				{
					printf("%i. class id: %i\n", i, entity.get_classid());
				}
			}
		}


		//bhop
		if (GetAsyncKeyState(VK_SPACE))
		{
			if (local.get_flags() & FL_ONGROUND)
				g_engine.force_jump();
		}


		//glow
		for (size_t x = 0; x <= g_engine.get_max_players(); x++)
		{
			c_base_entity entity = g_engine.get_entity_from_index(x);
			if (entity.address)
			{
				if (entity.get_team() == local.get_team())
					g_engine.glow_entity(entity, team);
				else
					g_engine.glow_entity(entity, enemy);
			}
		}
			

		Sleep(1);
	}

	return 0;
}