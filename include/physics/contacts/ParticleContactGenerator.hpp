#ifndef FRAYIEN_PARTICLECONTACTGENERATOR
#define FRAYIEN_PARTICLECONTACTGENERATOR

#include <vector>

#include "physics/contacts/ParticleContact.hpp"

class ParticleContactGenerator
{
private:
    
public:
    ParticleContactGenerator() = default;
    virtual ~ParticleContactGenerator() = default;

    virtual void addContact(std::vector<ParticleContact> & contacts) const = 0;
};

#endif // FRAYIEN_PARTICLECONTACTGENERATOR
