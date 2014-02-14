#ifndef MODEL_CLICHE_COMPTE_PAYEUR_HPP
#define MODEL_CLICHE_COMPTE_PAYEUR_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "model/data/ComptePayeurData.hpp"

namespace model {
namespace cliche {

class ComptePayeur final
{
    
public:
    
    explicit ComptePayeur(std::shared_ptr<model::data::ComptePayeurData> const& data);
    
    uint64_t getId() const {
        return _data->getId();
    }
    
    std::string const& getIdFonc() const {
        return _data->getIdFonc();
    }
    
private:

    std::shared_ptr<model::data::ComptePayeurData> _data;

};

} // namespace cliche
} // namespace model

#endif
