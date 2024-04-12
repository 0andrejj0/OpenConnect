# python -m grpc_tools.protoc -I.   --python_out=. --grpc_python_out=. ./types.proto
# python -m grpc_tools.protoc -I.   --python_out=. --grpc_python_out=. ./service.proto


import grpc

import service_pb2
import service_pb2_grpc

import types_pb2
import types_pb2_grpc

import time

with grpc.insecure_channel("localhost:8081") as channel:
    stub = service_pb2_grpc.ServerStub(channel)

    # push clipboard
    stub.PostClipboard(types_pb2.ClipboardEntry(clipboardText="KEK"))


    # send notification
    notification = types_pb2.NotificationEntry()
    setattr(notification, 'from', 'Random push')
    notification.data = 'Posted cringe'
    notifications = types_pb2.NotificationAggregate()
    notifications.notifications.extend([notification])
    stub.PostNotifications(request=notifications)

    # push key
    keys = types_pb2.KeyboardAggregate()
    keys.events.extend([types_pb2.KeyboardEntry(key=115)])
    stub.PostKey(keys)

    # push clipboard
    stub.PostClipboard(types_pb2.ClipboardEntry(clipboardText="My text))))"))

    # readClipboard
    clipboard = stub.ReadClipboard(types_pb2.Void())
    print(clipboard)

    # push text file
    stub.PostFile(types_pb2.File(name="file.txt", file=b'12345\n'))

    # send notification
    notification = types_pb2.NotificationEntry()
    setattr(notification, 'from', 'message')
    notification.data = ')))'
    notifications = types_pb2.NotificationAggregate()
    notifications.notifications.extend([notification])
    stub.PostNotifications(request=notifications)

    notification = types_pb2.NotificationEntry()
    setattr(notification, 'from', 'ЛКЛ')
    notification.data = 'lol'
    notifications = types_pb2.NotificationAggregate()
    notifications.notifications.extend([notification])
    stub.PostNotifications(request=notifications)
