#include <Logger.hpp>
#include <iomanip> 

logging::Logger::Logger(std::string filename, LogLevel level) {
	m_logLevel = level; // Установка уровня логирования
	m_fileStream.open(filename.c_str(), std::ios::out | std::ios::app); // Открытие файла для записи
}

logging::Logger::~Logger() {
	m_fileStream.close(); // Закрытие файла
}

void logging :: Logger::setLogLevel(LogLevel level) { // Метод изменение уровня логирования
	m_logLevel = level;
}

void logging :: Logger::log(std::string message, LogLevel level) {
	if (level >= m_logLevel) { // Нужно ли записывать сообщение в лог
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);

		std::string logLevelString = getLogLevelString(level); // Получение строки для уровня логирования
		std::time_t now = std::time(nullptr); // Получение текущего времени в секундах
		std::tm tm_ = *std::localtime(&now); // Преобразование текущего времени в структуру std::tm
		char buffer[100]; // Создание буфера для хранения отформатированной даты и времени
		std::strftime(buffer, 100, "%d.%m.%Y %H:%M:%S", &tm); // Форматирование даты и времени и запись в буфер
		std::string timestamp(buffer); // Преобразование отформатированной даты и времени в string
		std::string logMessage = "[" + timestamp + "] " + "[" + logLevelString + "] " + message; // Формирование сообщения лога
		if (level >= LogLevel::Error) {
			std::cerr << logMessage << std::endl;
		}
		else {
			std::cout << logMessage << std::endl;
		}
		m_fileStream << logMessage << std::endl; // Запись сообщения в файл

	}
}

std::string logging:: Logger::getLogLevelString(LogLevel level) { // Метод для получения строки, соотвествующей уровню логирования
	switch (level) {
	case LogLevel::Debug:
		return "\033[0;34m" + std::string("DEBUG") + "\033[0m";
	case LogLevel::Info:
		return "\033[0;32m" + std::string("INFO") + "\033[0m";
	case LogLevel::Warning:
		return "\033[0;33m" + std::string("WARNING") + "\033[0m";
	case LogLevel::Error:
		return "\033[0;31m" + std::string("ERROR") + "\033[0m";
	case LogLevel::Fatal:
		return "\033[1;31m" + std::string("FATAL") + "\033[0m";
	default:
		return std::string("");
	}
}
