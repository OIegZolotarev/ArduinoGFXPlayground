#include <chrono>

std::chrono::steady_clock::time_point start_time;
long long gFrameTime;
long long gTimeElapsed;

void timerStart()
{
    start_time = std::chrono::steady_clock::now();
}



void timerUpdate()
{
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    

    gFrameTime = duration.count();
    gTimeElapsed += gFrameTime;

    // printf("%lf %d\n", (double)gTimeElapsed / 1000.f, gFrameTime);

    start_time = std::chrono::steady_clock::now();
}

float frameTime()
{  
    return (float)gFrameTime / 1000;
}

unsigned long millis()
{
    return (unsigned long)gTimeElapsed;
}