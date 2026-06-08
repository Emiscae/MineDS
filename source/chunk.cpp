#include "render.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128





#include <cstdint>
#include <cmath>
#include <vector>

enum BlockID {
    AIR = 0,
    STONE = 1,
    GRASS = 2,
    DIRT = 3,
    LOG = 17,    // Madeira do tronco
    LEAVES = 18, // Folhas
    SAND = 12,
    WATER = 9
};

const int CHUNK_X = 16;
const int CHUNK_Z = 16;
const int CHUNK_Y = 128;

class JavaRandom {
private:
    uint64_t seed;
    static const uint64_t multiplier = 0x5DEECE66DL;
    static const uint64_t addend = 0xBL;
    static const uint64_t mask = (1ULL << 48) - 1;

public:
    JavaRandom(uint64_t initial_seed) {
        setSeed(initial_seed);
    }

    void setSeed(uint64_t initial_seed) {
        seed = (initial_seed ^ multiplier) & mask;
    }

    int next(int bits) {
        seed = (seed * multiplier + addend) & mask;
        return static_cast<int>(seed >> (48 - bits));
    }

    int nextInt(int bound) {
        if ((bound & -bound) == bound)
            return static_cast<int>((bound * static_cast<uint64_t>(next(31))) >> 31);
        
        int bits, val;
        do {
            bits = next(31);
            val = bits % bound;
        } while (bits - val + (bound - 1) < 0);
        return val;
    }

    double nextDouble() {
        return ((static_cast<uint64_t>(next(26)) << 27) + next(27)) / static_cast<double>(1ULL << 53);
    }
};

class NoiseGeneratorPerlin {
public:
    std::vector<int> permutations;
    double xo, yo, zo;

    NoiseGeneratorPerlin(JavaRandom& rand) {
        permutations.resize(512);
        xo = rand.nextDouble() * 256.0;
        yo = rand.nextDouble() * 256.0;
        zo = rand.nextDouble() * 256.0;

        for (int i = 0; i < 256; i++) {
            permutations[i] = i;
        }

        for (int i = 0; i < 256; i++) {
            int j = rand.nextInt(256 - i) + i;
            int temp = permutations[i];
            permutations[i] = permutations[j];
            permutations[j] = temp;
            permutations[i + 256] = permutations[i];
        }
    }

    double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    double generateNoise(double x, double y, double z) {
        x += xo; y += yo; z += zo;
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        x -= std::floor(x); y -= std::floor(y); z -= std::floor(z);
        double u = fade(x), v = fade(y), w = fade(z);

        int A = permutations[X] + Y;
        int AA = permutations[A] + Z;
        int AB = permutations[A + 1] + Z;
        int B = permutations[X + 1] + Y;
        int BA = permutations[B] + Z;
        int BB = permutations[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(permutations[AA], x, y, z),
                                        grad(permutations[BA], x - 1, y, z)),
                                lerp(u, grad(permutations[AB], x, y - 1, z),
                                        grad(permutations[BB], x - 1, y - 1, z))),
                        lerp(v, lerp(u, grad(permutations[AA + 1], x, y, z - 1),
                                        grad(permutations[BA + 1], x - 1, y, z - 1)),
                                lerp(u, grad(permutations[AB + 1], x, y - 1, z - 1),
                                        grad(permutations[BB + 1], x - 1, y - 1, z - 1))));
    }
};

class NoiseGeneratorOctaves {
public:
    std::vector<NoiseGeneratorPerlin> generatorCollection;
    int octaves;

public:
    NoiseGeneratorOctaves(JavaRandom& rand, int octavesCount) {
        octaves = octavesCount;
        generatorCollection.reserve(octavesCount);
        for (int i = 0; i < octavesCount; i++) {
            generatorCollection.push_back(NoiseGeneratorPerlin(rand));
        }
    }

    void generateNoiseOctaves(double* noiseArray, int xOffset, int yOffset, int zOffset, 
                              int xSize, int ySize, int zSize, 
                              double xScale, double yScale, double zScale) {
        
        for (int i = 0; i < xSize * ySize * zSize; i++) {
            noiseArray[i] = 0.0;
        }

        double amplitude = 1.0;
        double frequency = 1.0;

        for (int o = 0; o < octaves; o++) {
            double freqX = xScale * frequency;
            double freqY = yScale * frequency;
            double freqZ = zScale * frequency;

            int index = 0;
            for (int x = 0; x < xSize; x++) {
                double posX = (xOffset + x) * freqX;
                for (int z = 0; z < zSize; z++) {
                    double posZ = (zOffset + z) * freqZ;
                    for (int y = 0; y < ySize; y++) {
                        double posY = (yOffset + y) * freqY;
                        
                        double noise = generatorCollection[o].generateNoise(posX, posY, posZ);
                        noiseArray[index] += noise * amplitude;
                        index++;
                    }
                }
            }
            
            amplitude /= 2.0;
            frequency *= 2.0;
        }
    }
}; // CORREÇÃO: A classe termina aqui!



void generateTerrain(int chunkX, int chunkZ, uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE], 
                     NoiseGeneratorOctaves& noiseGen1, 
                     NoiseGeneratorOctaves& noiseGen2, 
                     NoiseGeneratorOctaves& noiseGen3) {
    int sizeX = 5;
    int sizeY = 17;
    int sizeZ = 5;
    
    // Matriz 3D explícita para evitar erros de indexação linear
    double densityMap[5][5][17];
    
    double xScale = 684.412;
    double yScale = 684.412;
    double zScale = 684.412;
    
    // 1. Geramos e combinamos os ruídos diretamente nas coordenadas da grade 5x17x5
    for (int x = 0; x < sizeX; x++) {
        double posX = (chunkX * 4 + x) * xScale;
        double posX_sel = (chunkX * 4 + x) * (xScale / 80.0);
        
        for (int z = 0; z < sizeZ; z++) {
            double posZ = (chunkZ * 4 + z) * zScale;
            double posZ_sel = (chunkZ * 4 + z) * (zScale / 80.0);
            
            for (int y = 0; y < sizeY; y++) {
                double posY = y * yScale;
                double posY_sel = y * (yScale / 160.0);
                
                double noise1 = noiseGen1.generatorCollection[0].generateNoise(posX, posY, posZ);
                double noise2 = noiseGen2.generatorCollection[0].generateNoise(posX, posY, posZ);
                double seletor = (noiseGen3.generatorCollection[0].generateNoise(posX_sel, posY_sel, posZ_sel) / 10.0) + 0.5;
                
                double densidadeBase = 0.0;
                if (seletor < 0.0) {
                    densidadeBase = noise1;
                } else if (seletor > 1.0) {
                    densidadeBase = noise2;
                } else {
                    densidadeBase = noise1 + (noise2 - noise1) * seletor;
                }
                
                double offsetY = (static_cast<double>(y) - 8.0) * 4.0; 
                densidadeBase -= offsetY;
                
                densityMap[x][z][y] = densidadeBase;
            }
        }
    }

    // 2. Interpolação Trilinear com Delta Incremental mapeada para Array 3D nativo
    for (int br_x = 0; br_x < 4; br_x++) {
        for (int br_z = 0; br_z < 4; br_z++) {
            for (int br_y = 0; br_y < 16; br_y++) {
                
                double y000 = densityMap[br_x + 0][br_z + 0][br_y + 0];
                double y001 = densityMap[br_x + 0][br_z + 1][br_y + 0];
                double y100 = densityMap[br_x + 1][br_z + 0][br_y + 0];
                double y101 = densityMap[br_x + 1][br_z + 1][br_y + 0];
                
                double y010 = densityMap[br_x + 0][br_z + 0][br_y + 1];
                double y011 = densityMap[br_x + 0][br_z + 1][br_y + 1];
                double y110 = densityMap[br_x + 1][br_z + 0][br_y + 1];
                double y111 = densityMap[br_x + 1][br_z + 1][br_y + 1];

                double dY00 = (y010 - y000) * 0.125;
                double dY01 = (y011 - y001) * 0.125;
                double dY10 = (y110 - y100) * 0.125;
                double dY11 = (y111 - y101) * 0.125;

                double vY00 = y000;
                double vY01 = y001;
                double vY10 = y100;
                double vY11 = y101;

                for (int y = 0; y < 8; y++) {
                    double dX0 = (vY10 - vY00) * 0.25;
                    double dX1 = (vY11 - vY01) * 0.25;

                    double vX0 = vY00;
                    double vX1 = vY01;

                    for (int x = 0; x < 4; x++) {
                        double dZ = (vX1 - vX0) * 0.25;
                        double densidade = vX0;

                        for (int z = 0; z < 4; z++) {
                            int absX = br_x * 4 + x;
                            int absY = br_y * 8 + y;
                            int absZ = br_z * 4 + z;

                            // Atribuição direta utilizando as novas coordenadas indexadas da matriz 3D
                            if (densidade * 100.0 > 0.0) {
                                Chunk[absX][absY][absZ] = STONE;
                            } else if (absY <= 63) {
                                Chunk[absX][absY][absZ] = WATER;
                            } else {
                                Chunk[absX][absY][absZ] = AIR;
                            }

                            densidade += dZ;
                        }
                        vX0 += dX0;
                        vX1 += dX1;
                    }
                    vY00 += dY00;
                    vY01 += dY01;
                    vY10 += dY10;
                    vY11 += dY11;
                }
            }
        }
    }
}

void replaceBlocksForBiome(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]) {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            
            int quantidadeTerra = -1;
            
            for (int y = 127; y >= 0; y--) {
                uint8_t blocoAtual = Chunk[x][y][z];
                
                if (blocoAtual == AIR || blocoAtual == WATER) {
                    quantidadeTerra = -1; // Se atingir fluidos ou ar, desliga o gerador de solo
                } 
                else if (blocoAtual == STONE) {
                    if (quantidadeTerra == -1) {
                        // FIX DO INFERNO: Verifica se acima é Ar Real da superfície para evitar gerar terra em tetos de cavernas
                        bool expostoAOSuporteReal = (y == 127 || Chunk[x][y + 1][z] == AIR || Chunk[x][y + 1][z] == WATER);
                        
                        if (expostoAOSuporteReal) {
                            if (y >= 64) {
                                Chunk[x][y][z] = GRASS;
                                quantidadeTerra = 3;
                            } else {
                                Chunk[x][y][z] = SAND;
                                quantidadeTerra = 3;
                            }
                        }
                    } 
                    else if (quantidadeTerra > 0) {
                        quantidadeTerra--;
                        Chunk[x][y][z] = DIRT;
                    }
                }
            }
        }
    }
}

void growTree(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE], int baseX, int baseZ, int baseY) {
    int alturaTronco = 5;

    // 1. Gera a copa de folhas
    for (int y = baseY + alturaTronco - 3; y <= baseY + alturaTronco; y++) {
        int distDoTopo = y - (baseY + alturaTronco);
        int raioFolhas = 1 - distDoTopo / 2;

        for (int x = baseX - raioFolhas; x <= baseX + raioFolhas; x++) {
            for (int z = baseZ - raioFolhas; z <= baseZ + raioFolhas; z++) {
                if (x >= 0 && x < 16 && z >= 0 && z < 16 && y >= 0 && y < 128) {
                    if (y < baseY + alturaTronco || (abs(x - baseX) + abs(z - baseZ) <= raioFolhas)) {
                        if (Chunk[x][y][z] == AIR) {
                            Chunk[x][y][z] = LEAVES;
                        }
                    }
                }
            }
        }
    }

    // 2. Ergue o tronco de madeira
    for (int y = 0; y < alturaTronco; y++) {
        if (baseY + y < 128) {
            Chunk[baseX][baseY + y][baseZ] = LOG;
        }
    }
}

void generateSimpleCave(int chunkX, int chunkZ, uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE], uint64_t worldSeed) {
    uint64_t chunkSeed = (worldSeed * (chunkX * 31237LL + chunkZ * 43211LL)) ^ 0xDEADBEEF;
    JavaRandom caveRand(chunkSeed);
    
    if (caveRand.nextInt(100) > 75) return;
    
    double cx = 0.0; 
    double cy = static_cast<double>(40 + caveRand.nextInt(20));
    double cz = static_cast<double>(caveRand.nextInt(16));
    
    int comprimentoTunnel = 35 + caveRand.nextInt(20);
    double raioBase = 1.5 + (caveRand.nextDouble() * 1.5);
    
    double yaw = caveRand.nextDouble() * M_PI * 2.0; 
    double pitch = (caveRand.nextDouble() - 0.5) * 0.2; 
    
    for (int i = 0; i < comprimentoTunnel; i++) {
        cx += cos(yaw) * cos(pitch);
        cy += sin(pitch);
        cz += sin(yaw) * cos(pitch);
        
        yaw += (caveRand.nextDouble() - 0.5) * 0.5;
        pitch = (pitch * 0.75) + (caveRand.nextDouble() - 0.5) * 0.2;
        
        double raioAtual = raioBase * (0.8 + sin(i * 0.5) * 0.2);
        
        int minX = std::max(0, (int)(cx - raioAtual));
        int maxX = std::min(15, (int)(cx + raioAtual));
        int minY = std::max(1, (int)(cy - raioAtual));
        int maxY = std::min(127, (int)(cy + raioAtual));
        int minZ = std::max(0, (int)(cz - raioAtual));
        int maxZ = std::min(15, (int)(cz + raioAtual));
        
        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                for (int z = minZ; z <= maxZ; z++) {
                    
                    double dx = x - cx;
                    double dy = y - cy;
                    double dz = z - cz;
                    
                    if ((dx * dx + dy * dy + dz * dz) < (raioAtual * raioAtual)) {
                        // Escavação direta desobstruída
                        if (Chunk[x][y][z] == STONE || Chunk[x][y][z] == DIRT) {
                            Chunk[x][y][z] = AIR;
                        }
                    }
                }
            }
        }
    }
}

void chunkStart(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]){
    /*for(int x = 0; x < CHUNK_SIZE; x++){
        for(int y = 0; y < CHUNK_HEIGHT; y++){
            for(int z = 0; z < CHUNK_SIZE; z++){
                if(y > 14) Chunk[x][y][z] = 1;
                else if(y < 15 && y > 3) Chunk[x][y][z] = 2;
                else Chunk[x][y][z] = 7;
            }
        }
    }
    Chunk[3][3][3] = 0;
    Chunk[3][2][2] = 0;*/
    uint64_t seed = 64315;
    JavaRandom rand(seed);
    
    NoiseGeneratorOctaves noiseGen1(rand, 16);
    NoiseGeneratorOctaves noiseGen2(rand, 16);
    NoiseGeneratorOctaves noiseGen3(rand, 8);

    generateTerrain(0, 0, Chunk, noiseGen1, noiseGen2, noiseGen3);
    generateSimpleCave(0, 0, Chunk, seed);
    replaceBlocksForBiome(Chunk);

    int xArvore = 2 + rand.nextInt(12); // Sorteia de 2 a 13 usando o seed do Java
    int zArvore = 2 + rand.nextInt(12); // Sorteia de 2 a 13 usando o seed do Java
    int yArvore = -1;

    for (int y = 127; y >= 0; y--) {
        if (Chunk[xArvore][y][zArvore] == GRASS) {
            yArvore = y + 1;
            break;
        }
    }

    if (yArvore != -1) {
        growTree(Chunk, xArvore, zArvore, yArvore);
    } 
}

void chunkRender(
    uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE],
    int playerX, int playerY, int playerZ,
    float camRotY
) {
    int minY = std::max(0, playerY - 16);
    int maxY = std::min(CHUNK_HEIGHT - 1, playerY + 16);

    // direção da câmera
    float angleRad = camRotY * (M_PI / 180.0f);

    float dirX = sinf(angleRad);
    float dirZ = -cosf(angleRad);

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {

            float dx = x - playerX;
            float dz = z - playerZ;

            float dist = sqrtf(dx * dx + dz * dz);

            // evita divisão por zero e renderiza perto do player
            if (dist < 4.0f) {
                for (int y = minY; y <= maxY; y++) {
                    if (Chunk[x][y][z] != 0) {
                        renderBlock(x, y, z, Chunk);
                    }
                }
                continue;
            }

            // normaliza direção até a coluna
            dx /= dist;
            dz /= dist;

            // produto escalar
            float dot = (dx * dirX) + (dz * dirZ);

            // culling: ignora o que está atrás
            // 0.0 = 180°
            // 0.5 = ~120° de FOV
            // 0.7 = ~90° de FOV
            if (dot < 0.3f) {
                continue;
            }

            for (int y = minY; y <= maxY; y++) {
                if (Chunk[x][y][z] != 0) {
                    renderBlock(x, y, z, Chunk);
                }
            }
        }
    }
}
