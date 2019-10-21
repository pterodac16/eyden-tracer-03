#include "Scene.h"

#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "PrimTriangle.h"

#include <fstream>

void CScene::ParseOBJ(const std::string& fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;

	std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE ---

	std::vector<Vec3f> vertices;
	std::string line, v, f;
	std::ifstream cowFile (fileName);

	while (!cowFile.eof()) {
		std::getline(cowFile, line);
		if (line[0] == 'v') {
			std::istringstream iss(line);
			Vec3f vertex;
			iss >> v >> vertex[0] >> vertex[1] >> vertex[2];
			std::cout << vertex << std::endl;
			vertices.push_back(vertex);
		}
		else if (line[0] == 'f') {
			std::istringstream iss(line);
			int a, b, c;
			iss >> f >> a >> b >> c;
			a--; b--; c--;
			std::cout << vertices[a] << " " << vertices[b] << " " << vertices[c] << std::endl;
			Add(std::make_shared<CPrimTriangle>(vertices[a], vertices[b], vertices[c], pShader));
		}
	}

	std::cout << "Finished Parsing" << std::endl;
}

