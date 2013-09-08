#include "model/cliche/Pfi.hpp"

#include "boost/foreach.hpp"

#include "model/cliche/ComptePayeur.hpp"

using boost::shared_ptr;

using date::LocalDateTime;
using model::data::ComptePayeurData;
using model::data::PfiData;


namespace model {
namespace cliche {

void Pfi::build(PfiData& data, date::LocalDateTime const& date) {
    _data = &data;
    
    BOOST_FOREACH(shared_ptr<ComptePayeurData> const& comptePayeurData, data.getComptesPayeurs()) {
        if (comptePayeurData->getValidite()->contains(date)) {
            _comptePayeur.build(*comptePayeurData, date);
            break;
        }
        // FIXME Controler la presence du compte payeur
    }
}

} // namespace cliche
} // namespace model
