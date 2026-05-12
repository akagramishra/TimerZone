#include "src/app.hpp"

int main()
{
    Application app;
    app.Init();
    app.RunLoop();
    app.Shutdown();

    return 0;
}