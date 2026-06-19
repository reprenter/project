#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Portfolio.h"

#include <string>

class FileManager {
public:
    void savePortfolio(const Portfolio<Position>& portfolio, const std::string& filename) const;
    void loadPortfolio(Portfolio<Position>& portfolio, const std::string& filename) const;
};

#endif
