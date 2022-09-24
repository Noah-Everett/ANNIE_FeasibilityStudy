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

void interactionRates( string t_fileName )
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

    THStack* stack = new THStack( "", "" );

    // Total events
    TH1F* hist_tot = new TH1F( "", "Simulated interactions in ANNIE;" "E_{#nu} [MeV];" "Events / 50 MeV / 5E12 POT;", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) hist_tot->Fill( interactions[ i ].Ev );
    hist_tot->GetXaxis()->SetTitleOffset( 1.15 );
    hist_tot->GetYaxis()->SetTitleOffset( 1.0 ); // 1.0 (linear), 1.4 (log)
    
    // nu_e events
    TH1F* hist_nu_e = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_e ) hist_nu_e->Fill( interactions[ i ].Ev );
    
    // nu_-e events
    TH1F* hist_nu_e_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_e_anti ) hist_nu_e_anti->Fill( interactions[ i ].Ev );
    
    // nu_mu events
    TH1F* hist_nu_mu = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_mu ) hist_nu_mu->Fill( interactions[ i ].Ev );
    
    // nu_-mu events
    TH1F* hist_nu_mu_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_mu_anti ) hist_nu_mu_anti->Fill( interactions[ i ].Ev );
    
    // nu_tau events
    TH1F* hist_nu_tau = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_tau ) hist_nu_tau->Fill( interactions[ i ].Ev );
    
    // nu_-tau events
    TH1F* hist_nu_tau_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_tau ) hist_nu_tau_anti->Fill( interactions[ i ].Ev );
    
    // CC events
    TH1F* hist_cc = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].cc && interactions[ i ].neu == pdg_nu_mu ) hist_cc->Fill( interactions[ i ].Ev );
    
    // qel events
    TH1F* hist_qel = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel && interactions[ i ].neu == pdg_nu_mu ) hist_qel->Fill( interactions[ i ].Ev );
    
    // // CCQE in fiducial volume
    // TH1F* hist_yes = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    // for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel && interactions[ i ].cc && interactions[ i ].neu == pdg_nu_mu ) hist_yes->Fill( interactions[ i ].Ev );
    
    hist_tot        ->SetLineColor( kBlack );
    hist_nu_e       ->SetLineColor( kBlack );
    hist_nu_e_anti  ->SetLineColor( kBlack );
    hist_nu_mu      ->SetLineColor( kBlack );
    hist_nu_mu_anti ->SetLineColor( kBlack );
    hist_nu_tau     ->SetLineColor( kBlack );
    hist_nu_tau_anti->SetLineColor( kBlack );
    hist_cc         ->SetLineColor( kBlack );
    hist_qel        ->SetLineColor( kBlack );
    hist_tot        ->SetLineWidth( 1 );
    hist_nu_e       ->SetLineWidth( 1 );
    hist_nu_e_anti  ->SetLineWidth( 1 );
    hist_nu_mu      ->SetLineWidth( 1 );
    hist_nu_mu_anti ->SetLineWidth( 1 );
    hist_nu_tau     ->SetLineWidth( 1 );
    hist_nu_tau_anti->SetLineWidth( 1 );
    hist_cc         ->SetLineWidth( 1 );
    hist_qel        ->SetLineWidth( 1 );
    hist_tot        ->SetFillColorAlpha( kBlack   , 0.2 );
    hist_nu_e       ->SetFillColorAlpha( kBlue-9  , 1.0 );
    hist_nu_e_anti  ->SetFillColorAlpha( kCyan-3  , 1.0 );
    hist_nu_mu      ->SetFillColorAlpha( kRed+2   , 1.0 );
    hist_nu_mu_anti ->SetFillColorAlpha( kRed-7   , 1.0 );
    hist_nu_tau     ->SetFillColorAlpha( kGreen+2 , 1.0 );
    hist_nu_tau_anti->SetFillColorAlpha( kGreen-6 , 1.0 );
    hist_cc         ->SetFillColorAlpha( kOrange-7, 1.0 );
    hist_qel        ->SetFillColorAlpha( kOrange-2, 1.0 );

    // stack->Add( hist_tot         );
    stack->Add( hist_nu_mu       );
    stack->Add( hist_cc          );
    stack->Add( hist_qel         );
    stack->Add( hist_nu_mu_anti  );
    stack->Add( hist_nu_e        );
    stack->Add( hist_nu_e_anti   );
    stack->Add( hist_nu_tau_anti );
    stack->Add( hist_nu_tau      );

    string label_tot         = "Total events in ANNIE"; label_tot         += " (" + to_string( int( hist_tot        ->GetEntries() ) ) + " events)";
    string label_nu_e        = "#nu_{e}"              ; label_nu_e        += " (" + to_string( int( hist_nu_e       ->GetEntries() ) ) + " events)";
    string label_nu_e_anti   = "#bar{#nu}_{e}"        ; label_nu_e_anti   += " (" + to_string( int( hist_nu_e_anti  ->GetEntries() ) ) + " events)";
    string label_nu_mu       = "#nu_{#mu}"            ; label_nu_mu       += " (" + to_string( int( hist_nu_mu      ->GetEntries() ) ) + " events)";
    string label_nu_mu_anti  = "#bar{#nu}_{#mu}"      ; label_nu_mu_anti  += " (" + to_string( int( hist_nu_mu_anti ->GetEntries() ) ) + " events)";
    string label_nu_tau      = "#nu_{#tau}"           ; label_nu_tau      += " (" + to_string( int( hist_nu_tau     ->GetEntries() ) ) + " events)";
    string label_nu_tau_anti = "#bar{#nu}_{#tau}"     ; label_nu_tau_anti += " (" + to_string( int( hist_nu_tau_anti->GetEntries() ) ) + " events)";
    string label_cc          = "CC events"            ; label_cc          += " (" + to_string( int( hist_cc         ->GetEntries() ) ) + " events)";
    string label_qel         = "QE events"            ; label_qel         += " (" + to_string( int( hist_qel        ->GetEntries() ) ) + " events)";

    TLegend* legend = new TLegend(0.444126,0.589474,0.863897,0.871579);
    legend->AddEntry( hist_tot        , label_tot        .c_str() );
    legend->AddEntry( hist_nu_e       , label_nu_e       .c_str() );
    legend->AddEntry( hist_nu_e_anti  , label_nu_e_anti  .c_str() );
    legend->AddEntry( hist_nu_mu      , label_nu_mu      .c_str() );
    legend->AddEntry( hist_nu_mu_anti , label_nu_mu_anti .c_str() );
    legend->AddEntry( hist_nu_tau     , label_nu_tau     .c_str() );
    legend->AddEntry( hist_nu_tau_anti, label_nu_tau_anti.c_str() );
    legend->AddEntry( hist_cc         , label_cc         .c_str() );
    legend->AddEntry( hist_qel        , label_qel        .c_str() );
    legend->SetBorderSize( 0 );

    double scale = double(1E20) / double(5E20);
    double norm  = hist_tot->GetEntries() * scale;
    cout << "Scale: " << scale << endl;

    hist_tot        ->DrawNormalized( ""    , hist_tot        ->GetEntries() * scale );
    hist_nu_mu      ->DrawNormalized( "same", hist_nu_mu      ->GetEntries() * scale );
    hist_cc         ->DrawNormalized( "same", hist_cc         ->GetEntries() * scale );
    hist_qel        ->DrawNormalized( "same", hist_qel        ->GetEntries() * scale );
    hist_nu_mu_anti ->DrawNormalized( "same", hist_nu_mu_anti ->GetEntries() * scale );
    hist_nu_e       ->DrawNormalized( "same", hist_nu_e       ->GetEntries() * scale );
    hist_nu_e_anti  ->DrawNormalized( "same", hist_nu_e_anti  ->GetEntries() * scale );
    hist_nu_tau_anti->DrawNormalized( "same", hist_nu_tau_anti->GetEntries() * scale );
    hist_nu_tau     ->DrawNormalized( "same", hist_nu_tau     ->GetEntries() * scale );

    // hist_tot->Draw();
    // hist_nu_e       ->Draw( "Same" );
    // hist_nu_e_anti  ->Draw( "Same" );
    // hist_nu_mu      ->Draw( "Same" );
    // hist_nu_mu_anti ->Draw( "Same" );
    // hist_nu_tau     ->Draw( "Same" );
    // hist_nu_tau_anti->Draw( "Same" );
    // hist_cc         ->Draw( "Same" );
    // hist_qel        ->Draw( "Same" );
    // stack ->Draw( "nostack noclear same" );
    legend->Draw( "Same" );

    gPad->RedrawAxis();

    canvas->SaveAs("interactionRates.pdf");
}
