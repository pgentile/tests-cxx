#ifndef MODEL_CLICHE_COMPTE_HPP
#define MODEL_CLICHE_COMPTE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "model/data/CompteData.hpp"

namespace model {
namespace cliche {

class Compte final
{
    
public:
    
    explicit Compte(std::shared_ptr<model::data::CompteData> const& data);
    
    uint64_t getId() const {
        return _data->getId();
    }
    
    std::string const& getIdFonc() const {
        return _data->getIdFonc();
    }
    
private:

    std::shared_ptr<model::data::CompteData> _data;

};

} // namespace cliche
} // namespace model

#endif
