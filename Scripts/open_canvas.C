using std::string;

void open_canvas( string file ) {
    TFile* f = new TFile( file.c_str() );
    TCanvas* c = ( TCanvas* )f->Get( "c1" );
    c->Draw();
}