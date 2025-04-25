#include <cstdlib>
#include <ctime>
#include "App.hpp"
#include "Core/Context.hpp"


int main(int, char**) {
    auto context = Core::Context::GetInstance();
    context->SetWindowIcon("assets/ref/textures/FE1.png");
    App app;

    std::srand(static_cast<unsigned>(std::time(0))); //initial random seed

    while (!context->GetExit()) {
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
    }
    return 0;
}
