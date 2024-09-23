package com.example.pc_control

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.pc_control.ui.theme.PCConnectTheme
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.foundation.border
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import android.net.Uri
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.text.BasicTextField
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.unit.dp
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.pc_control.ui.theme.PCConnectTheme
import androidx.compose.foundation.border
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.os.Build
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.pc_control.ui.theme.PCConnectTheme
import androidx.compose.foundation.border
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.compose.foundation.text.BasicTextField
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.activity.compose.setContent
import androidx.compose.ui.platform.LocalContext
import android.Manifest
import android.app.Activity
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.activity.result.ActivityResultLauncher



class MainActivity : ComponentActivity() {

    private lateinit var notificationPermissionLauncher: ActivityResultLauncher<String>

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Initialize the permission launcher
        notificationPermissionLauncher = registerForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted ->
            if (isGranted) {
                // Permission granted, proceed to send the notification
                sendNotification(this)
            } else {
                // Handle permission denial
                println("Notification permission denied")
            }
        }

        createNotificationChannel()

        setContent {
            PCConnectTheme {
                val navController = rememberNavController()
                Navigation(navController = navController, context = this)
            }
        }
    }

    // Function to request notification permission
    fun requestNotificationPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.POST_NOTIFICATIONS)
                != PackageManager.PERMISSION_GRANTED
            ) {
                // Request the permission
                notificationPermissionLauncher.launch(Manifest.permission.POST_NOTIFICATIONS)
            } else {
                // Permission already granted, send the notification
                sendNotification(this)
            }
        } else {
            // For devices below Android 13, no permission is needed
            sendNotification(this)
        }
    }

    // Function to create a notification channel for Android 8.0 and higher
    private fun createNotificationChannel() {
        val name = "Notification Sync Channel"
        val descriptionText = "Channel for notification sync"
        val importance = NotificationManager.IMPORTANCE_DEFAULT
        val channel = NotificationChannel("CHANNEL_ID", name, importance).apply {
            description = descriptionText
        }
        val notificationManager: NotificationManager =
            getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        notificationManager.createNotificationChannel(channel)
    }
}

//.setSmallIcon(R.drawable.ic_launcher_foreground)
// Function to send a notification
fun sendNotification(context: Context) {
    // Check if notification permission is granted (for Android 13 and higher)
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
        if (ContextCompat.checkSelfPermission(context, Manifest.permission.POST_NOTIFICATIONS) != PackageManager.PERMISSION_GRANTED) {
            // Permission is not granted, so log or handle accordingly
            println("Notification permission not granted")
            return
        }
    }

    // Build and send the notification
    val builder = NotificationCompat.Builder(context, "CHANNEL_ID")
        .setSmallIcon(R.drawable.ic_launcher_foreground)  // Ensure ic_notification is in drawable folder
        .setContentTitle("Notification Synchronized")
        .setContentText("It is ON")  // The message for your notification
        .setPriority(NotificationCompat.PRIORITY_HIGH)  // High priority for heads-up notifications
        .setDefaults(NotificationCompat.DEFAULT_ALL)  // Include sound, vibration, etc.
        .setAutoCancel(true)  // Automatically dismiss when clicked

    // Send the notification
    with(NotificationManagerCompat.from(context)) {
        notify(1001, builder.build())  // Notification ID can be unique
    }
}


// ViewModel to hold the selected device state
class DeviceViewModel : ViewModel() {
    var selectedDevice by mutableStateOf<String?>(null) // Allow null values for no device selected
        private set

    fun selectDevice(device: String?) {
        selectedDevice = device
    }
}



@Composable
fun Navigation(navController: NavHostController, context: Context) {
    NavHost(navController = navController, startDestination = "home") {
        composable("home") {
            HomeScreen(navController = navController, context = context)
        }
        composable("connect_pc") {
            ConnectWithPCScreen(navController = navController)
        }
        composable("send_files") {
            SendFilesScreen(navController = navController)
        }
        composable("remote_input") {
            RemoteInputScreen(navController = navController)
        }
    }
}

@Composable
fun HomeScreen(navController: NavHostController, context: Context) {
    val backgroundColor = Color(0xFFF2F7F2)
    val primaryTextColor = Color(0xFF1B2432)
    val buttonColor = Color(0xFFEF8275)
    val cardBackgroundColor = Color(0xFF403F4C)

    var isBufferSynchronized by rememberSaveable { mutableStateOf(false) }
    var isNotificationsSynchronized by rememberSaveable { mutableStateOf(false) }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(backgroundColor)
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Text(
            text = "Sync Your Phone and PC",
            style = MaterialTheme.typography.headlineLarge.copy(
                fontWeight = FontWeight.Bold,
                fontSize = 24.sp,
                color = primaryTextColor
            ),
            modifier = Modifier.padding(bottom = 16.dp)
        )

        Card(
            shape = RoundedCornerShape(8.dp),
            colors = CardDefaults.cardColors(containerColor = cardBackgroundColor),
            elevation = CardDefaults.cardElevation(8.dp),
            modifier = Modifier
                .fillMaxWidth()
                .padding(bottom = 32.dp)
        ) {
            Column(
                modifier = Modifier.padding(16.dp),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                Text(
                    text = "Easily sync your phone and computer",
                    style = MaterialTheme.typography.bodyLarge.copy(
                        fontWeight = FontWeight.Bold,
                        fontSize = 18.sp,
                        color = Color.White
                    ),
                    modifier = Modifier.padding(bottom = 8.dp)
                )
                Text(
                    text = "Manage files, control inputs, and sync notifications with a simple connection.",
                    style = MaterialTheme.typography.bodyMedium.copy(
                        fontSize = 16.sp,
                        lineHeight = 22.sp,
                        color = Color.White
                    ),
                    modifier = Modifier.padding(bottom = 8.dp)
                )
            }
        }

        ActionButton("Connect with PC", buttonColor, primaryTextColor) {
            navController.navigate("connect_pc")
        }

        Spacer(modifier = Modifier.height(16.dp))

        ActionButton("Send Files", buttonColor, primaryTextColor) {
            navController.navigate("send_files")
        }

        Spacer(modifier = Modifier.height(16.dp))

        ActionButton("Remote Input", buttonColor, primaryTextColor) {
            navController.navigate("remote_input")
        }

        Spacer(modifier = Modifier.height(16.dp))

        SynchronizeButton(
            isSynchronized = isBufferSynchronized,
            label = "Synchronize Buffer",
            onSyncClick = { isBufferSynchronized = !isBufferSynchronized },
            context = context
        )

        Spacer(modifier = Modifier.height(16.dp))

        SynchronizeButton(
            isSynchronized = isNotificationsSynchronized,
            label = "Synchronize Notifications",
            onSyncClick = {
                isNotificationsSynchronized = !isNotificationsSynchronized
                if (!isNotificationsSynchronized) {
                    sendNotification(context)
                }
            },
            context = context
        )
    }
}



@Composable
fun ConnectWithPCScreen(navController: NavHostController, deviceViewModel: DeviceViewModel = viewModel()) {
    val backgroundColor = Color(0xFFF2F7F2)
    val primaryTextColor = Color(0xFF1B2432)
    val buttonColor = Color(0xFFEF8275)
    val connectedButtonColor = Color(0xFF81C784)  // Softer green for connected
    val cardBackgroundColor = Color(0xFF403F4C)

    // Use ViewModel to store connected device state
    val selectedDevice = deviceViewModel.selectedDevice
    val devices = listOf("andrejj-pc")

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(backgroundColor)
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = "Available Devices",
            style = MaterialTheme.typography.headlineLarge.copy(
                fontWeight = FontWeight.Bold,
                fontSize = 24.sp,
                color = primaryTextColor
            ),
            modifier = Modifier.padding(bottom = 16.dp)
        )

        // Move the connected device to the top
        selectedDevice?.let { connectedDevice ->
            DeviceCard(
                deviceName = connectedDevice,
                isConnected = true,
                cardBackgroundColor = cardBackgroundColor,
                buttonColor = connectedButtonColor,
                isHighlighted = true,
                onConnectClick = { deviceViewModel.selectDevice(null) }  // Disconnect
            )
            Spacer(modifier = Modifier.height(16.dp))
            // Wider line separator between the connected and available devices
            Divider(color = Color.Gray, thickness = 5.dp)  // Increased thickness to 5.dp
            Spacer(modifier = Modifier.height(16.dp))
        }

        // Display other devices that are not connected
        devices.filter { it != selectedDevice }.forEach { device ->
            DeviceCard(
                deviceName = device,
                isConnected = false,
                cardBackgroundColor = cardBackgroundColor,
                buttonColor = buttonColor,
                onConnectClick = { deviceViewModel.selectDevice(device) }  // Connect
            )
            Spacer(modifier = Modifier.height(16.dp))
        }

        Spacer(modifier = Modifier.weight(1f))

        // Back button
        ActionButton("Back", buttonColor, Color.White) {
            navController.popBackStack()
        }
    }
}





@Composable
fun SendFilesScreen(navController: NavHostController) {
    var selectedFileUri by remember { mutableStateOf<Uri?>(null) }
    val filePickerLauncher = rememberLauncherForActivityResult(
        contract = ActivityResultContracts.GetContent()
    ) { uri: Uri? ->
        selectedFileUri = uri
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Text(text = "Choose a file to send", style = MaterialTheme.typography.bodyLarge.copy(fontWeight = FontWeight.Bold))

        Spacer(modifier = Modifier.height(16.dp))

        // Choose File Button with new color 1B2432
        Button(
            onClick = { filePickerLauncher.launch("*/*") },
            modifier = Modifier.fillMaxWidth().height(50.dp),
            shape = RoundedCornerShape(50),
            colors = ButtonDefaults.buttonColors(containerColor = Color(0xFF1B2432)) // New color for Choose File button
        ) {
            Text(text = "Choose File", style = MaterialTheme.typography.bodyLarge.copy(color = Color.White))
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Reflect that a file has been selected
        selectedFileUri?.let {
            Text(text = "File Selected: ${it.path}", style = MaterialTheme.typography.bodyMedium.copy(color = Color(0xFF4CAF50)))
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Conditionally show the "Send" button if a file is selected
        if (selectedFileUri != null) {
            Button(
                onClick = { /* Implement file sending logic here */ },
                modifier = Modifier.fillMaxWidth().height(50.dp),
                shape = RoundedCornerShape(50),
                colors = ButtonDefaults.buttonColors(containerColor = Color(0xFF81C784)) // Color for Send button
            ) {
                Text(text = "Send", style = MaterialTheme.typography.bodyLarge.copy(color = Color.White))
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Back Button with swapped color
        Button(
            onClick = { navController.popBackStack() },
            modifier = Modifier.fillMaxWidth().height(50.dp),
            shape = RoundedCornerShape(50),
            colors = ButtonDefaults.buttonColors(containerColor = Color(0xFFEF8275)) // Color for Back button
        ) {
            Text(text = "Back", style = MaterialTheme.typography.bodyLarge.copy(color = Color.White))
        }
    }
}






@Composable
fun RemoteInputScreen(navController: NavHostController) {
    val backgroundColor = Color(0xFFF2F7F2)
    val primaryTextColor = Color(0xFF1B2432)
    val buttonColor = Color(0xFFEF8275)
    val inputFieldColor = Color(0xFF403F4C)

    // State for the text input
    var text by remember { mutableStateOf("") }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(backgroundColor)
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Spacer(modifier = Modifier.weight(1f))  // Spacer to push the content into the center

        Text(
            text = "Type Text to Send to PC",
            style = MaterialTheme.typography.headlineLarge.copy(
                fontWeight = FontWeight.Bold,
                fontSize = 24.sp,
                color = primaryTextColor
            ),
            modifier = Modifier.padding(bottom = 16.dp)
        )

        // Text input field where letters are sent immediately to PC
        BasicTextField(
            value = "",  // Always keep the field empty
            onValueChange = { newText ->
                if (newText.isNotEmpty()) {
                    sendTextToPC(newText)  // Send the typed letter to the PC
                }
            },
            modifier = Modifier
                .fillMaxWidth()
                .height(150.dp)  // Adjust height as needed
                .background(inputFieldColor, shape = RoundedCornerShape(8.dp))
                .padding(16.dp),
            textStyle = LocalTextStyle.current.copy(color = Color.Transparent)  // Invisible text
        )

        Spacer(modifier = Modifier.weight(1f))  // Spacer to push the content and the back button lower

        // Back button positioned lower
        ActionButton("Back", buttonColor, Color.White) {
            navController.popBackStack()
        }
    }
}

// Function to send text to the PC (placeholder)
fun sendTextToPC(text: String) {
    println("Sending text to PC: $text")
}

/*@Composable
fun SynchronizeButtonVariant1(isSynchronized: Boolean, onSyncClick: () -> Unit) {
    val buttonColor = if (isSynchronized) Color(0xFF81C784) else Color(0xFFEF8275)
    val textColor = if (isSynchronized) Color.White else Color(0xFF1B2432)
    val buttonText = if (isSynchronized) "hm" else "Synchronize Buffer and Notifications"

    // Reduced button height to match the previous smaller size
    Button(
        onClick = onSyncClick,
        modifier = Modifier
            .fillMaxWidth()
            .height(50.dp),  // Smaller button height
        colors = ButtonDefaults.buttonColors(containerColor = buttonColor),
        shape = RoundedCornerShape(50),  // Keep the rounded shape
        elevation = ButtonDefaults.buttonElevation(8.dp)
    ) {
        Text(
            text = buttonText,
            style = MaterialTheme.typography.bodyLarge.copy(color = textColor)
        )
    }
}
*/

@Composable
fun SynchronizeButton(
    isSynchronized: Boolean,
    label: String,
    onSyncClick: () -> Unit,
    context: Context
) {
    val buttonColor = if (isSynchronized) Color(0xFF81C784) else Color(0xFFEF8275)
    val textColor = if (isSynchronized) Color.White else Color(0xFF1B2432)
    val buttonText = if (isSynchronized) "$label - Synchronized" else label

    Button(
        onClick = {
            onSyncClick()

            if (!isSynchronized) {
                // Log message to check if we reach this point
                println("Synchronizing Notifications...")

                // Request permission and send notification if not already synchronized
                (context as MainActivity).requestNotificationPermission()

                // Check if we have permission before sending
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                    if (ContextCompat.checkSelfPermission(context, Manifest.permission.POST_NOTIFICATIONS) == PackageManager.PERMISSION_GRANTED) {
                        sendNotification(context)
                        println("Notification sent")
                    } else {
                        println("Notification permission not granted")
                    }
                } else {
                    // For Android versions lower than 13
                    sendNotification(context)
                    println("Notification sent for Android below 13")
                }
            }
        },
        modifier = Modifier
            .fillMaxWidth()
            .height(50.dp),  // Consistent height with other buttons
        colors = ButtonDefaults.buttonColors(containerColor = buttonColor),
        shape = RoundedCornerShape(50),  // Matching rounded shape
        elevation = ButtonDefaults.buttonElevation(8.dp)
    ) {
        Text(
            text = buttonText,
            style = MaterialTheme.typography.bodyLarge.copy(color = textColor)
        )
    }
}

@Composable
fun DeviceCard(
    deviceName: String,
    isConnected: Boolean,
    cardBackgroundColor: Color,
    buttonColor: Color,
    isHighlighted: Boolean = false,
    onConnectClick: (() -> Unit)? = null
) {
    Card(
        shape = RoundedCornerShape(8.dp),
        colors = CardDefaults.cardColors(containerColor = cardBackgroundColor),
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp)
            .then(
                if (isHighlighted) Modifier.border(2.dp, Color(0xFFEF8275), RoundedCornerShape(8.dp)) else Modifier
            ),
        elevation = CardDefaults.cardElevation(4.dp)
    ) {
        Row(
            modifier = Modifier
                .padding(16.dp)
                .fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            Text(text = deviceName, style = MaterialTheme.typography.bodyLarge.copy(color = Color.White))

            Button(
                onClick = { onConnectClick?.invoke() },
                colors = ButtonDefaults.buttonColors(containerColor = if (isConnected) Color(0xFF81C784) else buttonColor)
            ) {
                Text(text = if (isConnected) "Connected" else "Connect")
            }
        }
    }
}




@Composable
fun ActionButton(text: String, backgroundColor: Color, textColor: Color, onClick: () -> Unit) {
    Button(
        onClick = onClick,
        modifier = Modifier
            .fillMaxWidth()
            .height(50.dp),  // Same height for all buttons
        colors = ButtonDefaults.buttonColors(containerColor = backgroundColor),
        shape = RoundedCornerShape(50),
        elevation = ButtonDefaults.buttonElevation(8.dp)
    ) {
        Text(text = text, style = MaterialTheme.typography.bodyLarge.copy(color = textColor))
    }
}

@Preview(showBackground = true)
@Composable
fun HomeScreenPreview() {
    val context = LocalContext.current
    val navController = rememberNavController()
    PCConnectTheme {
        HomeScreen(navController = navController, context = context)
    }
}

@Preview(showBackground = true)
@Composable
fun ConnectWithPCScreenPreview() {
    PCConnectTheme {
        ConnectWithPCScreen(navController = rememberNavController())
    }
}

@Preview(showBackground = true)
@Composable
fun SendFilesScreenPreview() {
    PCConnectTheme {
        SendFilesScreen(navController = rememberNavController())
    }
}
