package com.airblair.amp1;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;

import androidx.recyclerview.widget.RecyclerView;


import android.os.Bundle;

import android.view.MenuItem;

import com.google.android.material.navigation.NavigationView;

public class MainActivity extends AppCompatActivity implements
        NavigationView.OnNavigationItemSelectedListener {
    private DrawerLayout drawer;
    public static String currentContinent;
    public static CountryData myLists;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        myLists = new CountryData();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        drawer = findViewById(R.id.drawer_layout);
        NavigationView navigationView = findViewById(R.id.nav_view);

        navigationView.setNavigationItemSelectedListener(this);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        if(savedInstanceState == null) {
            currentContinent = "Africa";
            getSupportFragmentManager().beginTransaction().replace(R.id.container,
                    new ContinentFragment()).commit();
            navigationView.setCheckedItem(R.id.nav_Africa);
        }

    }

    @Override
    public void onBackPressed() {
        if(drawer.isDrawerOpen(GravityCompat.START)) {
        drawer.closeDrawer(GravityCompat.START);
        }
        else{
            super.onBackPressed();
        }
    }


    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem menuItem) {
        switch (menuItem.getItemId()){
            case R.id.nav_Africa:
                currentContinent = "Africa";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Asia:
                currentContinent = "Asia";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Europe:
                currentContinent = "Europe";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Na:
                currentContinent = "North America";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Oceania:
                currentContinent = "Oceania";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Sa:
                currentContinent = "South America";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
            case R.id.nav_Antarctica:
                currentContinent = "Antarctica";
                getSupportFragmentManager().beginTransaction().replace(R.id.container,
                        new ContinentFragment()).commit();
                break;
        }

        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
