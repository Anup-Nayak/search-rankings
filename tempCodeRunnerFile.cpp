nt argc, char const *argv[])
{
    vector<unsigned long long> v;
    ifstream file("mkgandhi_csv.csv");
    string word;
    string f;
    int ct=0;
    for (int i = 0; i < 309; ++i)
    {
        getline(file,word,',');
        getline(file,f);
        int h = hass(word);
        v.push_back(h);
        // if(v[h]) {cout<<word<<' '<<h<<' '<<v[h]<<endl; ct++;}
        // v[h]++;
    }
    sort(v.begin(), v.end());
    for(auto i:v){
        cout<<i<<' ';
    }
    return 0;
}