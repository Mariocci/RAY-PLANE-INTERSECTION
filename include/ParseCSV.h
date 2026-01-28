#pragma once
#include <vector>
#include <memory>
#include <string>

class GeometryBody;

std::vector<std::unique_ptr<GeometryBody>>
parseCSV(const std::string& filename);
