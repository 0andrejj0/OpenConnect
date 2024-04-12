import GRPC
import Foundation
import Network
import NIOCore
import NIOPosix
import SwiftProtobuf

final class GrpcIntegration {
		
		let host: String = "192.168.1.67"
		let port: Int = 8080
		
		var group: EventLoopGroup
		var client: GRPCClient?
		
		init() {
				self.group = MultiThreadedEventLoopGroup(numberOfThreads: 1)
				let channel = try? GRPCChannelPool.with(
						target: .host(self.host, port: self.port),
						transportSecurity:  .plaintext,
						eventLoopGroup: group
				)
				if let channel {
						client = GRPCAnyServiceClient(channel: channel)
				} else {
						print("Client don't be created")
						shutDown()
				}
		}
		
		func shutDown() {
				try? group.syncShutdownGracefully()
				try? client?.channel.close().wait()
		}
		
		func send(string: String) {
				let request = StringRequest.with {
						$0.name = string
				}
				let _: UnaryCall<StringRequest, StringRequest>? = client?.makeUnaryCall(
						path: Metadata.Methods.sendString.path,
						request: request
				)
				print("Sended")
		}
		
}

public struct StringRequest {

	public var name: String = String()

	public var unknownFields = SwiftProtobuf.UnknownStorage()

	public init() {}
}

fileprivate let _protobuf_package = "common.types"

extension StringRequest: @unchecked Sendable { }

extension StringRequest: SwiftProtobuf.Message, SwiftProtobuf._MessageImplementationBase, SwiftProtobuf._ProtoNameProviding {
		
		public static let protoMessageName: String = _protobuf_package + ".ClipboardEntry"
		public static let _protobuf_nameMap: SwiftProtobuf._NameMap = [
				1: .same(proto: "clipboardText"),
		]

		public mutating func decodeMessage<D: SwiftProtobuf.Decoder>(decoder: inout D) throws {
				while let fieldNumber = try decoder.nextFieldNumber() {

						switch fieldNumber {
								case 1: try { try decoder.decodeSingularStringField(value: &self.name) }()
								default: break
						}
				}
		}

		public func traverse<V: SwiftProtobuf.Visitor>(visitor: inout V) throws {
				if !self.name.isEmpty {
						try visitor.visitSingularStringField(value: self.name, fieldNumber: 1)
				}
				try unknownFields.traverse(visitor: &visitor)
		}

		public static func ==(lhs: StringRequest, rhs: StringRequest) -> Bool {
				if lhs.name != rhs.name {return false}
				if lhs.unknownFields != rhs.unknownFields {return false}
				return true
		}
}

public enum Metadata {
		
	public static let serviceDescriptor = GRPCServiceDescriptor(
		name: "Sender",
		fullName: "Sender",
		methods: [
			Metadata.Methods.sendString
		]
	)

	public enum Methods {
			
		public static let sendString = GRPCMethodDescriptor(
			name: "SendString",
			path: "/Sender/SendString",
			type: GRPCCallType.unary
		)
	}
}
