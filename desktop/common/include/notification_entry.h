#pragma once

#include <string>
#include <vector>

namespace openconnect {
    struct NotificationEntryCpp {
        std::string senderName;
        std::string text;
    };
    using NotificationAggregateCpp = std::vector<NotificationEntryCpp>;

}