int main(){


DIR *d;
struct dirent *dir;

d= opendir("/home/sayli/Sunbeam_internship");

if(!d)
perror("opendir");

while ((dir = readdir(d)) != NULL){

printf("%s\n, dir -> d_name");

}


closedir(d);

return 0;
}
