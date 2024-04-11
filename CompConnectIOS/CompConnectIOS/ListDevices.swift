import SwiftUI
import CFNetwork

struct ListDevices: View {
		
		@StateObject var converter: HostnameConverter = HostnameConverter()
		
		var body: some View {
				List {
						Section {
								ForEach(converter.hostnames, id: \.self) { hostname in
										NavigationLink {
												ListFeatures()
										} label: {
												LabeledContent {
														if hostname == "andrejj-pc" {
																Text("🐸")
														} else {
																Text("🦐")
														}
												} label: {
														Text(hostname)
												}
										}
								}
						}
				}
				.navigationBarTitle("Устройства")
		}
}

@MainActor
class HostnameConverter: ObservableObject {
		
		@Published var hostnames: [String] = []
		
		init() {
				hostnames(forIPv4Address: "192.168.1.1")
		}
		
		func hostnames(forIPv4Address address: String) {
				var result: UnsafeMutablePointer<addrinfo>?
				var hints = addrinfo()
				memset(&hints, 0, MemoryLayout.size(ofValue: hints))
				hints.ai_flags = AI_NUMERICHOST
				hints.ai_family = PF_UNSPEC
				hints.ai_socktype = SOCK_STREAM
				hints.ai_protocol = 0
				
				let errorStatus = getaddrinfo(address, nil, &hints, &result)
				guard errorStatus == 0 else {
						return
				}
				defer {
						freeaddrinfo(result)
				}
				
				guard let ai_addr = result?.pointee.ai_addr,
							let ai_addrlen = result?.pointee.ai_addrlen,
							let addressRef = CFDataCreate(nil, ai_addr, numericCast(ai_addrlen))
				else {
						return
				}
				
				let hostRef = CFHostCreateWithAddress(nil, addressRef)
				
				let succeeded = CFHostStartInfoResolution(hostRef.takeUnretainedValue(), .names, nil)
				guard succeeded else {
						return
				}
				
				var hostnames = [String]()
				if let hostnamesRef = CFHostGetNames(hostRef.takeUnretainedValue(), nil) {
						for currentIndex in 0..<CFArrayGetCount(hostnamesRef.takeUnretainedValue()) {
								let cfString = unsafeBitCast(CFArrayGetValueAtIndex(hostnamesRef.takeUnretainedValue(), currentIndex), to: CFString.self)
								let hostname = String(cfString)
								hostnames.append(hostname)
						}
				}
				
				print(hostnames)
				self.hostnames = ["andrejj-pc"] + hostnames
		}
}
