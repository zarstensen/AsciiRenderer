#include <Asciir.h>

#include "Background.h"
#include "GameLayer.h"

using namespace Asciir;

class AsciirBros : public ARApp
{
public:
	void start(const std::vector<std::string>& args) final override
	{
		std::filesystem::path p = std::filesystem::current_path();

		Renderer::setFont("DejaVu Sans Mono", { 8, 8 });

		// setup game layers
		pushLayer(new Background(Renderer::maxSize()));
		pushLayer(new GameLayer);

		pushOverlay(new Tools::ProfilingLayer(1024 * 1024 * 256));

		// setup renderer

		Renderer::setThreads();
		Renderer::thrd_tile_count = 32;
		Renderer::setTitle("Super Mario!");
		Renderer::setMinDT(DeltaTime(60).fps());
	}
};

AR_DEFAULT_ENTRYPOINT(AsciirBros);
