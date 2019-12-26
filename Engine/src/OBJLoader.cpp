#include "OBJLoader.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace Engine;

bool OBJLoader::loadOBJ(VAO* vao, const char* path){
    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec3> out_normals;

    FILE * file = fopen(path, "r");
    if( file == NULL ){
        std::cout << "Can't open file: " << path << std::endl;
        return false;
    }
    while( 1 ){

	    char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;

		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
			if (matches != 6){
				std::cout << "Can't parse file: " << path << std::endl;
				fclose(file);
				return false;
		    }
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			char miscBuffer[1000];
			fgets(miscBuffer, 1000, file);
		}

	}

	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[ vertexIndex -1];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		out_vertices.push_back(vertex);
		out_normals .push_back(normal);

	}
    /*
    GLfloat finalVertices[out_vertices.size()*3];
    GLfloat finalNormals[out_normals.size()*3];
    int j = 0;
    for (int i = 0; i < out_vertices.size()*3;i+=3){
        finalVertices[i] = out_vertices[j].x;
        finalVertices[i+1] = out_vertices[j].y;
        finalVertices[i+2] = out_vertices[j].z;

        finalNormals[i] = out_normals[j].x;
        finalNormals[i+1] = out_normals[j].y;
        finalNormals[i+2] = out_normals[j].z;

        j++;
    }
    */
    vao->addFloatBuffer(0,glm::value_ptr(out_vertices[0]),out_vertices.size(),3);
    vao->addFloatBuffer(1,glm::value_ptr(out_normals[0]),out_normals.size(),3);
    fclose(file);
    return true;
}
