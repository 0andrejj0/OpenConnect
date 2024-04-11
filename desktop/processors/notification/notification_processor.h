
#include "notification_entry.h"

#include <mutex>
#include <any>


namespace openconnect {

class NotificationProcessor {
public:
    NotificationProcessor();
    
public:
    int PushNotification(NotificationAggregateCpp&& notifications) noexcept;
    
private:
    std::any m_impl;
};

} // namespace openconnect
