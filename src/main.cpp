#include "App.hpp"
#include "Core/Context.hpp"
#include <chrono> // 時間計算
#include <cstdlib>
#include <ctime>
#include <thread> // sleep_for()

int main(int, char **) {

    auto context = Core::Context::GetInstance();
    context->SetWindowIcon("assets/ref/textures/FE1.png");
    App app;

    std::srand(static_cast<unsigned>(std::time(0))); // initial random seed

    const int targetFPS = 40;
    const int frameDelay = 1000 / targetFPS;

    while (!context->GetExit()) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        switch (app.GetCurrentState()) {
        case App::State::START:
            app.Start();
            break;
        case App::State::UPDATE:
            app.Update();
            break;
        case App::State::END:
            app.End();
            context->SetExit(true);
            break;
        }

        context->Update();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = frameEnd - frameStart;

        if (elapsedTime.count() < frameDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::duration<double, std::milli>(frameDelay - elapsedTime.count()))
                    .count()));
        }
    }
    return 0;
}
