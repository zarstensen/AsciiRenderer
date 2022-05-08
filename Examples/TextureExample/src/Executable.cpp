#include <Asciir.h>

using namespace Asciir;
using namespace AsciirLiterals;

class TileShader : public Shader2D
{
public:
	TileShader()
	{
		square_trans.setOrigin(square.centrePoint());
	}

	TermVert size() const final override
	{
		return {-1, -1};
	}

protected:
	Transform square_trans;
	Quad square = Quad(Asciir::Coord(30, 30), Asciir::Coord(100, 10));


	Tile readTile(const TermVert& coord, Coord, const DeltaTime& t, size_t frame) final override
	{

		Tile result;

		if (coord.y == 0 && coord.x < 10)
			result.symbol = std::to_string(coord.x)[0];
		else if(coord.x == 0 && coord.y < 10)
			result.symbol = std::to_string(coord.y)[0];

		/*if((coord.x + coord.y + frame) % 2)
			return Tile(BLACK8);
		else
			return Tile(IWHITE8);*/

		int size = 50;

		Real dst = std::sqrtf(std::pow((int)coord.x - size, 2) + std::pow((int)coord.y * 2 - size, 2));

		int circle_exp = (int)(t.seconds() * 5) % size;

		if (dst < circle_exp)
			result.background_colour = Colour((dst / (circle_exp)) * 255, (int)((dst / (circle_exp)) * 500) % 255, (int)((dst / (circle_exp)) * 100) % 255);
		/*else
		{
			square_trans.setRotation(t / 10_R);

			if(square.isInsideGrid(coord, square_trans))
				result.background_colour = IRED8;
		}*/

		return result;
	}
};



class Mandelbrod : public Shader2D
{
	TermVert size() const final
	{
		return TermVert(300, 300);
	}

	Coord squareImaginary(Coord number) const {
		return Coord(
			std::pow(number.x, 2) - std::pow(number.y, 2),
			2 * number.x * number.y
		);
	}

	Tile readTile(const TermVert& coord, Coord uv, const DeltaTime&, size_t) final override
	{
		Coord z = Coord(0,0);

		size_t maxIterations = 100;

		for(size_t i = 0; i < maxIterations; i++){
			z = squareImaginary(z) + uv;
			if(z.norm() > 2)
			{
				Colour col = Colour(((Real)i / (Real)maxIterations) * 255);
				return Tile(col);
			}
		}

		return Colour(255);
	}
};

class TextureLayer : public Asciir::Layer
{
	Ref<FileTexture> texture = FileTexture();
	Ref<Shader2D> shader = new TileShader();

	AudioSource rroll_aud = AudioSource("Sound.mp3");
	AudioPlayer rroll_player = AudioPlayer(rroll_aud);

	void onAdd() final
	{
		rroll_player.getSrc<sf::Music>().setLoop(true);
		// rroll_player.play();

		Renderer::setFontSize({ 2, 2 });
	}

	Mesh square = std::vector<Coords>({ Coords({{5, 0}, {10, 0}, {20, 10}, {0, 10}}), Coords({{6, 3}, {10, 3}, {11, 7}, {6, 7}}) });
	Transform texture_t;
	Real rot = 0_R;

	DeltaTime time_since_start = 0;

	void onEvent(Event& e)
	{
		if (e.getType() == KeyPressedEvent::getStaticType())
		{
			KeyPressedEvent& pevent = (KeyPressedEvent&)e;

			AR_INFO((size_t)pevent.getKeyCode(), "\t:\t", pevent.isRepeat());
		}
	}

	void onUpdate(DeltaTime delta_time) final
	{
		exit(0);
		ARApp::getApplication()->getTermRenderer().setTitle(std::to_string(delta_time.fps()));
		Renderer::clear();


		time_since_start = time_since_start + delta_time;

		int frame = time_since_start.seconds() * 25;

		std::stringstream name;

		name << "Video/VIDEO" << std::setfill('0') << std::setw(4) << frame + 1 << ".jpg";

		FileTexture rroll(name.str());

		// Renderer::submit(rroll.load());
		// Renderer::resize(rroll.size());
	}
};

void foo()
{}

class TextureExample : public Asciir::ARApp
{
public:
	void start(const std::vector<std::string>&) override
	{
		Renderer::setThreads();
		Renderer::thrd_tile_count = 128;

		pushLayer(new TextureLayer());
		pushOverlay(new Tools::ProfilingLayer(1024 * 1024));
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



/*int main()
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
}*/

AR_DEFAULT_ENTRYPOINT(TextureExample)

//struct EW : public Eigen::Vector2<Real>
//{
//	EW(Real a, Real b)
//		: Eigen::Vector2<Real>(a, b) {}
//};
//
//int main()
//{
//	constexpr size_t iter = 100000000000;
//	constexpr size_t aaaa = 2073600;
//
//	
//
//	duration start = getTime();
//
//	for (size_t i = 0; i < iter; i++)
//		Eigen::Vector2<Real> c = Eigen::Vector2<Real>(i, i);
//
//	duration end = getTime();
//
//	std::cout << "Eigen coord took: " << DeltaTime(end - start) << '\n';
//
//	start = getTime();
//
//	for (size_t i = 0; i < iter; i++)
//		Coord c = Coord(i, i);
//
//	end = getTime();
//
//	std::cout << "Custom coord took: " << DeltaTime(end - start) << '\n';
//
//}
