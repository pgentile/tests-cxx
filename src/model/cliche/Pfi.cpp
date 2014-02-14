#include "model/cliche/Pfi.hpp"

#include "model/cliche/ComptePayeur.hpp"

using namespace std;
using date::LocalDateTime;
using model::data::ComptePayeurData;
using model::data::PfiData;


namespace model {
namespace cliche {

Pfi::Pfi(shared_ptr<PfiData> const& data, LocalDateTime const& date):
    _data(data)
{
    if (!data->getValidite().contains(date)) {
        throw logic_error("PFI inactif a la date donnee");
    }
    
    for (auto const comptePayeurData: data->getComptesPayeurs()) {
        if (comptePayeurData->getValidite().contains(date)) {
            _comptePayeur = ComptePayeur(comptePayeurData);
            break;
        }
    }
    if (!_comptePayeur) {
        throw logic_error("Compte payeur non trouve");
    }
}

} // namespace cliche
} // namespace model
