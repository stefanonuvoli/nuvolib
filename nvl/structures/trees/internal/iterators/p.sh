for f in *.inl; do 
    mv -- "$f" "${f%.inl}.cpp"
done
