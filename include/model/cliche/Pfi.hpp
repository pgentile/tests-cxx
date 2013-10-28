#ifndef MODEL_CLICHE_PFI_HPP
#define MODEL_CLICHE_PFI_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "date/LocalDateTime.hpp"
#include "model/cliche/ComptePayeur.hpp"
#include "model/data/PfiData.hpp"


namespace model {
namespace cliche {

class Pfi
{
    
public:
    
    Pfi(std::shared_ptr<model::data::PfiData> const& data, date::LocalDateTime const& date);
    
    uint64_t getId() const {
        return _data->getId();
    }
    
    std::string const& getIdFonc() const {
        return _data->getIdFonc();
    }
    
    std::shared_ptr<ComptePayeur> getComptePayeur() const {
        return _comptePayeur;
    }

private:
    
    std::shared_ptr<model::data::PfiData> _data;
    
    std::shared_ptr<ComptePayeur> _comptePayeur;

};

} // namespace cliche
} // namespace model

#endif
