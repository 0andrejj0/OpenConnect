#include "notification_processor.h"

#include <any>
#include <spdlog/spdlog.h>
#include <stdexcept>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "DesktopNotificationManagerCompat.h"
    #include <NotificationActivationCallback.h>
    #include <windows.ui.notifications.h>

    using namespace ABI::Windows::Data::Xml::Dom;
    using namespace ABI::Windows::UI::Notifications;
    using namespace Microsoft::WRL;
#elif  defined(__linux)

#include <libnotify/notify.h>
    
#elif defined(__APPLE__)
    
#endif

namespace {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#elif  __linux

class NotificationProcessorImplLinux {
public:
    NotificationProcessorImplLinux() {
        auto ok = notify_init("OpenConnect");
        if (!ok)
            throw std::runtime_error("failed to init notifications");
    }
public:
    int PushNotification(openconnect::NotificationAggregateCpp&& notifications) {
        for (auto& notification : notifications) {
            NotifyNotification *notifReq = notify_notification_new(notification.senderName.c_str(), notification.text.c_str(), NULL);
            notify_notification_show(notifReq, NULL);
        }
        return 0;
    }
};
    
#elif __APPLE__
    
#endif
}

namespace openconnect {


    NotificationProcessor::NotificationProcessor() {
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            #elif  defined(__linux)
                m_impl.emplace<NotificationProcessorImplLinux>();
            #elif defined(__APPLE__)
            #endif
    }


    int NotificationProcessor::PushNotification(NotificationAggregateCpp&& notifications) noexcept {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        #elif  defined(__linux)
            auto& impl = std::any_cast<NotificationProcessorImplLinux&>(m_impl);
            return impl.PushNotification(std::move(notifications));
            
        #elif defined(__APPLE__)
            
        #endif
    }
}