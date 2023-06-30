#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

namespace logging {
enum class LogLevel {
	Debug,
	Info,
	Warning,
	Error,
	Fatal
};

class Logger {
public:
	Logger(std::string filename, LogLevel level = LogLevel::Info); // Конструктор принимает имя файла и уровень логирования
	~Logger(); // Закрытие файла
	void setLogLevel(LogLevel level); // Изменение уровня логирования
	void log(std::string message, LogLevel level = LogLevel::Info); // Запись в консоль и файл

private:
	LogLevel m_logLevel; // Уровень логирования

	std::ofstream m_fileStream; // Поток для записи в файл

	std::string getLogLevelString(LogLevel level);
};

}
