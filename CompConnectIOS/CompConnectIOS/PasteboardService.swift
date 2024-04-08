import UIKit

final class PasteboardService {
		
		init() {
				NotificationCenter.default.addObserver(
						self,
						selector: #selector(clipboardChanged),
						name: UIPasteboard.changedNotification,
						object: nil
				)
		}
		
		@objc
		private func clipboardChanged() {
				let pasteboardString: String? = UIPasteboard.general.string
				// UIPasteboard.general
				// send data to pasteboard of compik
		}
}
