using std::string;
using std::vector;

struct interaction {
    double Ev  ; // neutrino energy
    int    neu ; // neutrino pdg code
    int    tgt ; // target pdg code
    bool   qel ; // quasi-elastic stattering
    bool   cc  ; // charge current
    double vtxx; // x interaction vertex
    double vtxy; // y interaction vertex
    double vtxz; // z interaction vertex
};

void make_hist_interactionRates_ANNIEPhaseII( string t_fileName )
{   
    gROOT->ForceStyle(); // loads ANNIE plot style if enabled
    gStyle->SetOptStat(0); // dissable histogram's stats box

    // open tree
    TCanvas* canvas = new TCanvas();
    canvas->SetLogy();
    TFile  * file   = TFile::Open( t_fileName.c_str() );
    TTree  * tree   = ( TTree* )file->Get( "gst;1" );
    
    // pdg codes
    int pdg_nu_e        =  12;
    int pdg_nu_e_anti   = -12;
    int pdg_nu_mu       =  14;
    int pdg_nu_mu_anti  = -14;
    int pdg_nu_tau      =  16;
    int pdg_nu_tau_anti = -16;
    int pdg_Ar40        = 1000180400;
    int pdg_O16         = 1000080160;
    int pdg_Fe60        = 1000260560;

    // interaction info objects
    vector< interaction > interactions;
    interaction temp; // used to fill interactions

    // Used to determine hist limits
    double max_Ev  = -1;
    double min_Ev  = -1;

    tree->SetBranchAddress( "Ev"  , &temp.Ev   );
    tree->SetBranchAddress( "neu" , &temp.neu  );
    tree->SetBranchAddress( "tgt" , &temp.tgt  );
    tree->SetBranchAddress( "qel" , &temp.qel  );
    tree->SetBranchAddress( "cc"  , &temp.cc   );
    tree->SetBranchAddress( "vtxx", &temp.vtxx );
    tree->SetBranchAddress( "vtxy", &temp.vtxy );
    tree->SetBranchAddress( "vtxz", &temp.vtxz );

    for( Int_t i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );

        temp.Ev *= 1000; // GeV --> MeV

        if( max_Ev == -1 ) {
            max_Ev  = temp.Ev ;
            min_Ev  = temp.Ev ;
        }
        if( temp.Ev  > max_Ev  ) max_Ev  = temp.Ev ;
        if( temp.Ev  < min_Ev  ) min_Ev  = temp.Ev ;

        interactions.push_back( temp );
    } 

    int nBins_GeV = 20; // number of bins per GeV
    int nBins_x = ceil( max_Ev * nBins_GeV / 1000 ) - floor( min_Ev * nBins_GeV / 1000);

    // Total events
    TH1F* hist_tot = new TH1F( "", "MC truth interaction cuts;" "E_{#nu} [MeV];" "Events / 50 MeV / 5E12 POT;", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) hist_tot->Fill( interactions[ i ].Ev );
    hist_tot->GetXaxis()->SetTitleOffset( 1.15 );
    hist_tot->GetYaxis()->SetTitleOffset( 0.9  );
    
    // Events in Ar
    TH1F* hist_ar = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].tgt == pdg_Ar40 ) hist_ar->Fill( interactions[ i ].Ev );
    
    // Events in O16
    TH1F* hist_o = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].tgt == pdg_O16 ) hist_o->Fill( interactions[ i ].Ev );
    
    // Events in O16
    TH1F* hist_fe = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].tgt == pdg_Fe60 ) hist_fe->Fill( interactions[ i ].Ev );

    // CC events
    TH1F* hist_cc = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].cc ) hist_cc->Fill( interactions[ i ].Ev );
    
    // qel events
    TH1F* hist_qel = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel ) hist_qel->Fill( interactions[ i ].Ev );
    
    // CCQE in Ar
    TH1F* hist_yes = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel && interactions[ i ].cc && interactions[ i ].tgt == pdg_Ar40 ) hist_yes->Fill( interactions[ i ].Ev );
    
    hist_tot->SetLineColor( kBlack );
    hist_ar ->SetLineColor( kBlack );
    hist_o  ->SetLineColor( kBlack );
    hist_fe ->SetLineColor( kBlack );
    hist_cc ->SetLineColor( kBlack );
    hist_qel->SetLineColor( kBlack );
    hist_yes->SetLineColor( kBlack );
    hist_tot->SetLineWidth( 1 );
    hist_ar ->SetLineWidth( 1 );
    hist_o  ->SetLineWidth( 1 );
    hist_fe ->SetLineWidth( 1 );
    hist_cc ->SetLineWidth( 1 );
    hist_qel->SetLineWidth( 1 );
    hist_yes->SetLineWidth( 1 );
    hist_tot->SetFillColorAlpha( kBlack    , 0.2 );
    hist_ar ->SetFillColorAlpha( kGreen-2  , 1.0 );
    hist_o  ->SetFillColorAlpha( kCyan-2   , 1.0 );
    hist_fe ->SetFillColorAlpha( kRed+2    , 1.0 );
    hist_cc ->SetFillColorAlpha( kOrange-6 , 1.0 );
    hist_qel->SetFillColorAlpha( kOrange-2 , 1.0 );
    hist_yes->SetFillColorAlpha( kYellow-10, 1.0 );

    string label_tot = "Total Events in ANNIE"       ; label_tot += " (" + to_string( int( hist_tot->GetEntries() ) ) + " events)";
    string label_ar  = "Events in ^{40}Ar"           ; label_ar  += " (" + to_string( int( hist_ar ->GetEntries() ) ) + " events)";
    string label_o   = "Events in ^{16}O"            ; label_o   += " (" + to_string( int( hist_o  ->GetEntries() ) ) + " events)";
    string label_fe  = "Events in ^{60}Fe"           ; label_fe  += " (" + to_string( int( hist_fe ->GetEntries() ) ) + " events)";
    string label_cc  = "CC Events"                   ; label_cc  += " (" + to_string( int( hist_cc ->GetEntries() ) ) + " events)";
    string label_qel = "QE Events"                   ; label_qel += " (" + to_string( int( hist_qel->GetEntries() ) ) + " events)";
    string label_yes = "CCQE Events in ^{40}Ar"      ; label_yes += " (" + to_string( int( hist_yes->GetEntries() ) ) + " events)";

    TLegend* legend = new TLegend(0.444126,0.589474,0.863897,0.871579);
    legend->AddEntry( hist_tot, label_tot.c_str() );
    legend->AddEntry( hist_ar , label_ar .c_str() );
    legend->AddEntry( hist_o  , label_o  .c_str() );
    legend->AddEntry( hist_fe , label_fe .c_str() );
    legend->AddEntry( hist_cc , label_cc .c_str() );
    legend->AddEntry( hist_qel, label_qel.c_str() );
    legend->AddEntry( hist_yes, label_yes.c_str() );
    legend->SetBorderSize( 0 );

    double scale = double(1E20) / double(5E20);
    double norm  = hist_tot->GetEntries() * scale;
    cout << "Scale: " << scale << endl;

    hist_tot->DrawNormalized( ""    , hist_tot->GetEntries() * scale );
    hist_o  ->DrawNormalized( "same", hist_o  ->GetEntries() * scale );
    hist_cc ->DrawNormalized( "same", hist_cc ->GetEntries() * scale );
    hist_qel->DrawNormalized( "same", hist_qel->GetEntries() * scale );
    hist_ar ->DrawNormalized( "same", hist_ar ->GetEntries() * scale );
    hist_yes->DrawNormalized( "same", hist_yes->GetEntries() * scale );
    hist_fe ->DrawNormalized( "same", hist_fe ->GetEntries() * scale );
    legend->Draw( "Same" );

    gPad->RedrawAxis();

    canvas->SaveAs("interactionRates.pdf");
}
