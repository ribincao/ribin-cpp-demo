#include "logger.h"


int main() {
    Logger logger(Logger::LogMode::CONSOLE);
    logger.Debug("debug message");
    logger.Info("info message");
    logger.Warn("warn message");
    logger.Error("error message");
    logger.Fatal("fatal message");

    return 0;
}

