plugins {
    id("com.android.application")
    id("com.google.gms.google-services")
    id("com.google.firebase.crashlytics")
}

android {
    signingConfigs {
    getByName("debug") {
        val keystoreFile = file("C:\\sc-karn\\fff\\keystore_playmarket2.jks")
        if (keystoreFile.exists()) {
            storeFile = keystoreFile
            storePassword = "hayk2010"
            keyPassword = "hayk2010"
            keyAlias = "key0"
        }
    }
    create("release") {
        val keystoreFile = file("C:\\sc-karn\\fff\\keystore_playmarket2.jks")
        if (keystoreFile.exists()) {
            storeFile = keystoreFile
            keyAlias = "key0"
            storePassword = "hayk2010"
            keyPassword = "hayk2010"
        }
    }
}

    namespace = "com.rstarx.hexrays"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.rstarx.hexrays"
        minSdk = 26
        targetSdk = 34
        versionCode = 130
        versionName = "1.0"

        multiDexEnabled = true

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    splits {
        abi {
            isEnable = true
            reset()
            include("armeabi-v7a", "arm64-v8a")
            isUniversalApk = true
        }
    }

    buildTypes {
        getByName("debug") {
            isDebuggable = true
            isJniDebuggable = true
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("release")

            externalNativeBuild {
                cmake {
                    cppFlags += "-fvisibility=default"
                }
            }

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }

        getByName("release") {
            isDebuggable = false
            isJniDebuggable = false
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("release")

            externalNativeBuild {
                cmake {
                    cppFlags += "-fvisibility=hidden"
                }
            }

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
        }
    }

    ndkVersion = "26.2.11394342"

    lint {
        abortOnError = false
        checkReleaseBuilds = false
    }

    buildFeatures {
        prefab = true
        viewBinding = true
    }

    packaging {
        jniLibs {
            // สำคัญ: แก้ไข Warning และช่วยให้โหลด Library เกมบน Android 11+ เสถียรขึ้น
            useLegacyPackaging = true
            excludes += "META-INF/*"
        }
        resources {
            excludes += "META-INF/*"
        }
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))

    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    implementation(libs.navigation.fragment)
    implementation(libs.navigation.ui)

    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext.junit)
    androidTestImplementation(libs.espresso.core)

    implementation("com.github.amitshekhariitbhu:PRDownloader:1.0.2")
    implementation("com.akexorcist:round-corner-progress-bar:2.1.2")
    implementation(libs.volley)
    implementation(libs.sdp)

    implementation(platform(libs.firebase.bom))
    implementation(libs.firebase.analytics)
    implementation(libs.firebase.crashlytics.ndk)
    implementation(libs.firebase.messaging)

    implementation(libs.ini4j)
    implementation(libs.glide)
    implementation(libs.lifecycle.process)
    implementation(libs.paranoid)
    implementation(libs.shadowhook)
    implementation("commons-io:commons-io:2.11.0")
    implementation("org.apache.commons:commons-compress:1.21")
    implementation("com.squareup.okhttp3:okhttp:4.12.0")
}
