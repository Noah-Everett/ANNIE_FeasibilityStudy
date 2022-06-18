using std::string;
using std::vector;

void make_hist_position( string t_fileName )
{   
    TCanvas* canvas = new TCanvas();
    TFile  * file   = TFile::Open( t_fileName.c_str() );
    TTree  * tree   = ( TTree* )file->Get( "gst;1" );
    
    vector< double > vtx_x;
    vector< double > vtx_z;
    double cur_x;
    double cur_z;
    double max_x = 2;
    double max_z = 0;
    double min_x = 2;
    double min_z = 0;

    tree->SetBranchAddress( "vtxx", &cur_x );
    tree->SetBranchAddress( "vtxz", &cur_z );

    for( Int_t i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );
        vtx_x.push_back( cur_x );
        vtx_z.push_back( cur_z );
        if( cur_x > max_x ) max_x = cur_x;
        if( cur_z > max_z ) max_z = cur_z;
        if( cur_x < min_x ) min_x = cur_x;
        if( cur_z < min_z ) min_z = cur_z;
    } 

    cout << "Max-X: " << max_x << endl;
    cout << "Max-Z: " << max_z << endl;
    cout << "Min-X: " << min_x << endl;
    cout << "Min-Z: " << min_z << endl;

    TH2F* hist = new TH2F( "Event Positions", "Event Positions",
                           ( ceil( max_x ) - floor( min_x ) ) / 0.015625, floor( min_x ), ceil( max_x ),
                           ( ceil( max_z ) - floor( min_z ) ) / 0.015625, floor( min_z ), ceil( max_z ) );
    for( int i = 0; i < vtx_x.size(); i++ )
        hist->Fill( vtx_x[ i ], vtx_z[ i ] );

    // hist->GetXaxis()->SetTitleOffset( 1.15 );
    // hist->GetYaxis()->SetTitleOffset( 0.7 );

    // TProfile* profile = hist->ProfileX();
    // profile->SetLineColor( kRed );
    // profile->SetLineWidth( 3 );

    // TLegend* legend = new TLegend();
    // legend->AddEntry( profile, "Average curve", "l" );
    // legend->SetBorderSize( 0 );

    hist->Draw( "COLZ" );
    // profile->Draw( "Same" );
    // legend->Draw( "Same" );
}