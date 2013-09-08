#ifndef MODEL_CLICHE_COMPTEPAYEUR_HPP
#define MODEL_CLICHE_COMPTEPAYEUR_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "model/data/ComptePayeurData.hpp"

namespace model {
namespace cliche {

class ComptePayeur
{
    
public:
    
    void build(model::data::ComptePayeurData& data, date::LocalDateTime const& date);
    
private:

    model::data::ComptePayeurData* _data;

};

} // namespace cliche
} // namespace model

#endif
