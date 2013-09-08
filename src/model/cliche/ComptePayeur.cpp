#include "model/cliche/ComptePayeur.hpp"

using date::LocalDateTime;
using model::data::ComptePayeurData;


namespace model {
namespace cliche {

void ComptePayeur::build(ComptePayeurData& data, LocalDateTime const& date) {
    _data = &data;
}

} // namespace cliche
} // namespace model
