import UIKit
import Network

final class ViewController: UIViewController {

		var connection: NWConnection?
		var hostUDP: NWEndpoint.Host = "192.168.1.67"
		var portUDP: NWEndpoint.Port = 8080

		override func viewDidLoad() {
				super.viewDidLoad()

				// Hack to wait until everything is set up
				var x = 0
				while x < 1000000000 {
						x += 1
				}
				
				view.backgroundColor = .red
				//connectToUDP(hostUDP, portUDP)
		}

		func connectToUDP(_ hostUDP: NWEndpoint.Host, _ portUDP: NWEndpoint.Port) {
				// Transmited message:
				let messageToUDP = "/connection"

				self.connection = NWConnection(host: hostUDP, port: portUDP, using: .udp)

				self.connection?.stateUpdateHandler = { (newState) in
						print("This is stateUpdateHandler:")
						switch (newState) {
								case .ready:
										print("State: Ready\n")
										self.sendUDP(messageToUDP)
										self.receiveUDP()
								case .setup:
										print("State: Setup\n")
								case .cancelled:
										print("State: Cancelled\n")
								case .preparing:
										print("State: Preparing\n")
								default:
										print("ERROR! State not defined!\n")
						}
				}

				self.connection?.start(queue: .global())
		}

		func sendUDP(_ content: Data) {
				self.connection?.send(content: content, completion: NWConnection.SendCompletion.contentProcessed(({ (NWError) in
						if (NWError == nil) {
								print("Data was sent to UDP")
						} else {
								print("ERROR! Error when data (Type: Data) sending. NWError: \n \(NWError!)")
						}
				})))
		}

		func sendUDP(_ content: String) {
				let contentToSendUDP = content.data(using: String.Encoding.utf8)
				self.connection?.send(content: contentToSendUDP, completion: NWConnection.SendCompletion.contentProcessed(({ (NWError) in
						if (NWError == nil) {
								print("Data was sent to UDP")
						} else {
								print("ERROR! Error when data (Type: Data) sending. NWError: \n \(NWError!)")
						}
				})))
		}

		func receiveUDP() {
				let reqContent = "OK"
				self.connection?.receiveMessage { (data, context, isComplete, error) in
						if (isComplete) {
								print("Receive is complete")
								if (data != nil) {
										let backToString = String(decoding: data!, as: UTF8.self)
										print("Received message: \(backToString)")
								} else {
										print("Data == nil")
								}
						}
				}
		}
}

