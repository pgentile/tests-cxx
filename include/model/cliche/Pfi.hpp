#ifndef MODEL_CLICHE_PFI_HPP
#define MODEL_CLICHE_PFI_HPP

#include <stdint.h>
#include <string>
#include <vector>

#include "date/LocalDateTime.hpp"
#include "model/cliche/ComptePayeur.hpp"
#include "model/data/PfiData.hpp"


namespace model {
namespace cliche {

class Pfi
{
    
public:
    
    void build(model::data::PfiData& data, date::LocalDateTime const& date);
    
private:
    
    model::data::PfiData* _data;
    
    ComptePayeur _comptePayeur;

};

} // namespace cliche
} // namespace model

#endif
