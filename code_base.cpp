#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

// Function to configure AWS credentials for the session.
void configure_AWS_credentials(string& aws_access_key, string& aws_secret_key) {
    cout << "Enter the AWS access key : ";
    cin >> aws_access_key;
    cout << "Enter the AWS secret access key : ";
    cin >> aws_secret_key;
}

// Function to create an S3 bucket.
void create_bucket(const string& bucket_name, const string& region) {
    string command = "aws s3api create-bucket --bucket " + bucket_name;
    if (region != "us-east-1") {
        command += "--region " + region +
            "--create-bucket-configuration location_constraint=" + region;
    }
    int result = system(command.c_str());
    if (result == 0) {
        cout << "Bucket " << bucket_name << " created successfully." << endl;
    }
    else {
        cerr << "Error creating bucket: " << bucket_name << "." << endl;
    }
}

// Function to upload a file to the S3 bucket.
void upload_file_to_bucket(const string& bucket_name, const string& file_path) {
    string command = "aws s3 cp " + file_path + " s3://" + bucket_name + "/";
    int result = system(command.c_str());
    if (result == 0) {
        cout << "File " << file_path << " uploaded successfully to bucket." << endl;
    }
}

// Write policy to a file.
string write_policy_to_file(const string& policy) {
    string policy_file = "policy.json";
    ofstream out(policy_file);
    if (out.is_open()) {
        out << policy;
        out.close();
    }
    return policy_file;
}

// Function to set bucket policy.
void set_bucket_policy(const string& bucket_name, bool make_public) {
    string policy_command;
    if (make_public) {
        policy_command =
            "{\"Version\": \"2012-10-17\","
            "\"Statement\": ["
            "{"
            "\"Sid\": \"PublicReadGetObject\","
            "\"Effect\": \"Allow\","
            "\"Principal\": \"*\","
            "\"Action\": \"s3:GetObject\","
            "\"Resource\": \"arn:aws:s3:::" + bucket_name + "/*\""
            "}"
            "]"
            "}";
    } else {
        cerr << "Policy generation for non-public bucket is not implemented yet." << endl;
        return;
    }

    // Write the policy to a file and apply it.
    string policy_file = write_policy_to_file(policy_command);
    string command = "aws s3api put-bucket-policy --bucket " + bucket_name + " --policy file://" + policy_file;
    int result = system(command.c_str());
    if (result == 0) {
        cout << "Bucket policy for " << bucket_name << " updated successfully." << endl;
    }
    else {
        cerr << "Error setting bucket policy for " << bucket_name << "." << endl;
    }
}

int main() {
    string bucket_name = "visual-studio-bucket-iii";
    string region = "us-east-1";
    string file_path = "terraform_certification.png";
    bool make_public = true;

    // AWS credentials input.
    string aws_access_key, aws_secret_key;
    configure_AWS_credentials(aws_access_key, aws_secret_key);

    // Operations with temporary credentials.
    cout << "Creating S3 bucket..." << endl;
    create_bucket(bucket_name, region);

    cout << "Uploading file to bucket..." << endl;
    upload_file_to_bucket(bucket_name, file_path);

    cout << "Setting bucket policy..." << endl;
    set_bucket_policy(bucket_name, make_public);

    cout << "Operations completed." << endl;


    return 0;


}
