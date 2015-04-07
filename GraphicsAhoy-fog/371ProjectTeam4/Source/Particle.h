
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <vector>

using namespace std;
using namespace glm;
//###############################################################


struct Particle
{
    Particle()
        : m_Position(0)
        , m_Velocity(0)
		, m_Acceleration(0)
        , m_Color(0)
        , m_fRotate(0)
        , m_fAge(0)
        , m_fLifeTime(0)
    {}
 
    glm::vec3   m_Position; // Center point of particle
    glm::vec3   m_Acceleration; // Current particle Acceleration
    glm::vec3   m_Velocity; // Current particle velocity
    glm::vec4   m_Color;    // Particle color
    float       m_fRotate;  // Rotate the particle the center
    float       m_fSize;    // Size of the particle
    float       m_fAge;
    float       m_fLifeTime;
};
 
typedef std::vector<Particle> ParticleBuffer;