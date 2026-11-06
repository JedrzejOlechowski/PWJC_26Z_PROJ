#ifndef DATALOADER_H
#define DATALOADER_H

#include <filesystem>

#include "DataModel.h"

Dataset readDataset(const std::filesystem::path& filePath);

#endif
