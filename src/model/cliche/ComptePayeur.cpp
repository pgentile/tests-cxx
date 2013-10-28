#include "model/cliche/ComptePayeur.hpp"

using namespace std;
using date::LocalDateTime;
using model::data::ComptePayeurData;


namespace model {
namespace cliche {

ComptePayeur::ComptePayeur(shared_ptr<ComptePayeurData> const& data, LocalDateTime const& date):
    _data(data)
{
}

} // namespace cliche
} // namespace model
