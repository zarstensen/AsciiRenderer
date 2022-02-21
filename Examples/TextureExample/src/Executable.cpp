#include <Asciir.h>

using namespace Asciir;

class TileShader : public Shader2D
{

	Size2D size() const final override
	{
		return Renderer::size();
	}

	Tile readTile(const Size2D& coord, const DeltaTime&, const size_t& frame) const final override
	{
		if((coord.x + coord.y + frame) % 2)
			return Tile(BLACK8);
		else
			return Tile(IWHITE8);
	}
};

class TextureLayer : public Asciir::Layer
{
	Ref<FileTexture> texture = FileTexture();
	Ref<TileShader> shader = TileShader();
	const Path texture_path = "./test.cart";

	void onAdd() final
	{
		//texture->load(texture_path);
		std::cout << sizeof(Asciir::Tile) << '\n';
		std::cout << texture->size() << '\n';
	}

	void onUpdate(DeltaTime delta_time) final
	{
		Transform texture_t;
		texture_t.setPos({ 10, 10 });
		texture_t.setScale({ 1, 1 });
		texture_t.setOrigin((Coord)texture->size() / 2);

		Renderer::submitShader(shader);
		//Renderer::submitShader(texture, texture_t);
	}
};

class TextureExample : public Asciir::ARApp
{
public:
	TextureExample()
	{
		pushLayer(new TextureLayer());
	}
};

struct PositionComponent : public Component
{
	PositionComponent(int x = 0, int y = 0) : x(x), y(y) {}

	int x;
	int y;
};

struct DirectionComponent : public Component
{
	DirectionComponent(int d_x = 0, int d_y = 0) : d_x(d_x), d_y(d_y) {}

	int d_x;
	int d_y;
};

struct HealthComponent : public Component
{
	HealthComponent(int max_health = 0) : health(max_health) {}

	int health;

};

struct DamageComponent : public Component
{
	DamageComponent(int damage = 0) : damage(damage) {}

	int damage;

};

struct TargetComponent : public Component
{
	TargetComponent(int off_x = 0, int off_y = 0) : off_x(off_x), off_y(off_y) {}

	int off_x;
	int off_y;
};

class PlayerHit : public SystemEvent
{
public:
	PlayerHit(UID player_id, size_t damage_taken = 0) : SystemEvent(typeid(PlayerHit)), player_id(player_id), damage_taken(damage_taken) {}

	size_t damage_taken;
	UID player_id;
};

class PlayerSystem : public System
{
public:
	PlayerSystem()
	{
		requireComponents<HealthComponent>();
		subscribe<PlayerHit>();
	}

	void process(Scene& scene) override
	{
		std::cout << getComponent<HealthComponent>().health;
	}

	void onEventRecieve(SystemEvent& s_event) override
	{
		if (s_event.getEventType() == typeid(PlayerHit))
		{
			std::cout << "PLAYER WAS HIT\nLOST " << ((PlayerHit&) s_event).damage_taken << " HEALTH\n";
			s_event.getScene().getComponents<PositionComponent>(((PlayerHit&) s_event).player_id).x += 10;
		}
	}

};

class SniperSystem : public System
{
public:
	SniperSystem()
	{
		requireComponents<TargetComponent, DamageComponent, PositionComponent>();
	}

	void process(Scene& scene) override
	{
		std::cout << "SNIPER SHOT A BULLET\n";
		SceneView targets_view = getScene().getView<HealthComponent, PositionComponent>();

		for (ComponentView& target : targets_view)
		{
			PositionComponent& target_pos = target.get<PositionComponent>();
			TargetComponent& targeter_aim = getComponent<TargetComponent>();
			PositionComponent& targeter_pos = getComponent<PositionComponent>();

			if ((target_pos.x == targeter_pos.y + targeter_aim.off_x) && (target_pos.y == targeter_pos.y + targeter_aim.off_y))
			{
				target.get<HealthComponent>().health -= getComponent<DamageComponent>().damage;
				createEvent(PlayerHit(target.getEntUID(), getComponent<DamageComponent>().damage));
			}
		}
	}
};

auto player_blueprint = EntityBlueprint<PositionComponent, HealthComponent>(PositionComponent(), HealthComponent(100));
auto enemy_sniper_blueprint = EntityBlueprint<PositionComponent, TargetComponent, DamageComponent>(PositionComponent(), TargetComponent(), DamageComponent(10));



int main()
{
	Scene scene;
	
	UID player = scene.createEntity(player_blueprint);
	UID sniper = scene.createEntity(enemy_sniper_blueprint);

	SniperSystem sniper_system;
	sniper_system.addScene(scene);

	PlayerSystem player_system;
	player_system.addScene(scene);

	scene.getComponents<PositionComponent>(player).x = 10;
	scene.getComponents<PositionComponent>(player).y = 10;

	scene.getComponents<PositionComponent>(sniper).x = 20;
	scene.getComponents<PositionComponent>(sniper).y = 20;

	scene.getComponents<TargetComponent>(sniper).off_x = -10;
	scene.getComponents<TargetComponent>(sniper).off_y = -10;

	scene.getComponents<HealthComponent>(player).health = 2000;

	sniper_system.run();
	sniper_system.run();
}

//AR_DEFAULT_ENTRYPOINT(TextureExample)
