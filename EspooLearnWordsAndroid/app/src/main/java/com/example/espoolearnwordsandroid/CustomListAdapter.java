package com.example.espoolearnwordsandroid;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.view.View;
import android.widget.BaseAdapter;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class CustomListAdapter extends BaseAdapter
{
    private ArrayList<ListItem> listData;
    private LayoutInflater layoutInflater;

    public CustomListAdapter(Context aContext, ArrayList<ListItem> listData)
    {
        this.listData = listData;
        layoutInflater = LayoutInflater.from(aContext);
    }

    @Override
    public int getCount()
    {
        return listData.size();
    }

    @Override
    public Object getItem(int position)
    {
        return listData.get(position);
    }

    @Override
    public long getItemId(int position)
    {
        return position;
    }

    public View getView(int position, View v, ViewGroup vg)
    {
        ViewHolder holder;
        if(v == null)
        {
            v = layoutInflater.inflate(R.layout.list_row, null);
            holder = new ViewHolder(v);
            v.setTag(holder);
        }
        else
        {
            holder = (ViewHolder)v.getTag();
        }
        holder.polish_word.setText(listData.get(position).getPolishWord());
        holder.english_word.setText(listData.get(position).getEnglishWord());
        holder.pronunciation.setText(listData.get(position).getPronunciation());

        return v;
    }

    static class ViewHolder
    {
        public TextView polish_word;
        public TextView english_word;
        public TextView pronunciation;

        public ViewHolder(View v)
        {
            polish_word = (TextView) v.findViewById(R.id.polish_word);
            english_word = (TextView) v.findViewById(R.id.english_word);
            pronunciation = (TextView) v.findViewById(R.id.pronunciation);
        }
    }

}































