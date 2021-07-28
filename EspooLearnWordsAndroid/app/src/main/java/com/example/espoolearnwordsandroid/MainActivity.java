package com.example.espoolearnwordsandroid;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import java.util.ArrayList;
import java.util.HashMap;

public class MainActivity extends AppCompatActivity
{

    Intent intent;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button newItem = (Button)findViewById(R.id.newItem);

        newItem.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                intent = new Intent(MainActivity.this, NewPolishWordActivity.class);
                startActivity(intent);
            }
        });
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        DatabaseHandler db = new DatabaseHandler(this);
        ArrayList<HashMap<String,String>> wordList = db.GetWords();

        ListView lv = (ListView)findViewById(R.id.wordslist);
        ListAdapter adapter = new SimpleAdapter(MainActivity.this, wordList, R.layout.list_row, new String[]{DatabaseHandler.KEY_POLISH, DatabaseHandler.KEY_ENGLISH, DatabaseHandler.KEY_PRONUNCIATION}, new int[]{R.id.txtPol, R.id.txtEng, R.id.txtPron});
        lv.setAdapter(adapter);
    }
}



























