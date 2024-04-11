#include <string>
#include <vector>

namespace openconnect {
    struct NotificationEntry {
        std::string senderName;
        std::string text;
    };
    using NotificationAggregate = std::vector<NotificationEntry>;

}